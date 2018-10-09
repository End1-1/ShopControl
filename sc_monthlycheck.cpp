#include "sc_monthlycheck.h"
#include "sc_editgoods.h"
#include <QInputDialog>

void SC_MonthlyCheck::addGoods(Goods *g)
{
    int row = t->rowCount();
    if (!g)
        return;
    g->row = row;
    t->setRowCount(row + 1);
    for (int i = 0; i < t->columnCount(); i++)
        t->setItem(row, i, new QTableWidgetItem());
    t->item(row, 0)->setData(Qt::EditRole, g->code);
    t->item(row, 1)->setData(Qt::EditRole, g->type);
    t->item(row, 2)->setData(Qt::EditRole, g->name);
    t->item(row, 3)->setData(Qt::EditRole, g->qty[QTY_TILL]);
    t->item(row, 4)->setData(Qt::EditRole, g->qty[QTY_IN]);
    t->item(row, 5)->setData(Qt::EditRole, g->qty[QTY_OUT]);
    t->item(row, 6)->setData(Qt::EditRole, g->qty[QTY_STORE]);
    t->item(row, 7)->setData(Qt::EditRole, g->qty[QTY_FACT]);
    t->item(row, 8)->setData(Qt::EditRole, g->qty[QTY_DIFF]);

}

void SC_MonthlyCheck::getQty(int docType, int field, QDate d1, QDate d2)
{
    if (!db.openDB())
        return;
    if (!db.prepare("select sb.fgoods, sb.fqty from sth st, stb sb "
                 "where st.id=sb.fdoc and st.fdate between :date1 and :date2 "
                 "and st.ftype=:ftype"))
        return;
    db.bindValue(":date1", d1);
    db.bindValue(":date2", d2);
    db.bindValue(":ftype", docType);
    if (!db.execSQL())
        return;
    while (db.next()) {
        Goods *g = m_goods[db.v_int(0)];
        if (!g)
            continue;
        g->setQty(field, g->qty[field] + db.v_dbl(1));
    }
    db.closeDB();
}

int SC_MonthlyCheck::getIdByBarcode()
{
    int result = 0;
    QString code = QInputDialog::getText(this, tr("Barcode"), tr("Barcode"));
    if (!db.openDB())
        return 0;
    if (!db.prepare("select fgoods from barcode where fbarcode=:fbarcode"))
        return 0;
    db.bindValue(":fbarcode", code);
    if (!db.execSQL())
        return 0;
    if (db.next())
        result = db.v_int(0);
    db.closeDB();
    return result;
}


SC_MonthlyCheck::SC_MonthlyCheck(QWidget *parent) :
    SC_Grid(parent)
{
    setWindowTitle(tr("Monthly check"));
    t->setColumnCount(9);
    QList<int> widths;
    widths << 80 << 150 << 300 << 80 << 80 << 80 << 80 << 80 << 80;
    for (int i = 0; i < t->columnCount(); i++)
        t->setColumnWidth(i, widths.at(i));
    QStringList h;
    h << tr("Code") << tr("Type") << tr("Name")
      << tr("Till") << tr("In") << tr("Out")
      << tr("Counted") << tr("Fact") << tr("Diff");
    t->setHorizontalHeaderLabels(h);
    setFilter("date1", new DX_TableFilter(DX_TableFilter::wtDateEdit, tr("Date"), false, this));
    setFilter("date2", new DX_TableFilter(DX_TableFilter::wtDateEdit, "", false, this));
    f("date1")->setValue(QDate::currentDate());
    f("date2")->setValue(QDate::currentDate());
    QAction *a = toolBar()->addAction(QIcon(":/img/card.png"), tr("Open goods"));
    connect(a, &QAction::triggered, [this](bool v) {
        Q_UNUSED(v);
        SC_EditGoods *d = new SC_EditGoods(this);
        d->load(getId().toInt());
        d->exec();
        delete d;
    });
}

void SC_MonthlyCheck::aPrepareSql()
{
    t->clearContents();
    if (!db.openDB())
        return;
    if (!db.prepare("select g.id, t.fname, g.fname from goods g, groups t "
                 "where g.fgroup=t.id order by t.fname"))
        return;
    if (!db.execSQL())
        return;
    while (db.next()) {
        Goods *g = new Goods();
        g->code = db.v_int(0);
        g->type = db.v_str(1);
        g->name = db.v_str(2);
        g->qty[QTY_TILL] = 0;
        g->qty[QTY_IN] = 0;
        g->qty[QTY_OUT] = 0;
        g->qty[QTY_STORE] = 0.0;
        g->qty[QTY_FACT] = 0.0;
        g->qty[QTY_DIFF] = 0.0;
        connect(g, SIGNAL(qtyChanged()), this, SLOT(updateRow()));
        m_goods[g->code] = g;
    }
    QDate d1 = f("date1")->getDate();
    QDate d2 = f("date2")->getDate();
    if (!db.prepare("select id from sth where fdate=:fdate and ftype=:ftype"))
        return;
    db.bindValue(":fdate", d2);
    db.bindValue(":ftype", 4);
    if (!db.execSQL())
        return;
    if (db.next())
        m_docId = db.v_int(0);
    else {
        m_docId = db.genId("gen_sth_id");
        if (!db.prepare("insert into sth (id, fdate, ftype, fpartner) values (:id, :fdate, :ftype, :fpartner)"))
            return;
        db.bindValue(":id", m_docId);
        db.bindValue(":fdate", d2);
        db.bindValue(":ftype", 4);
        db.bindValue(":fpartner", 1);
        if (!db.execSQL())
            return;
    }
    getQty(4, QTY_TILL, d1, d1);
    getQty(2, QTY_IN, d1, d2);
    getQty(1, QTY_OUT, d1, d2);
    getQty(3, QTY_OUT, d1, d2);
    getQty(4, QTY_FACT, d2, d2);
    for (QMap<int, Goods*>::iterator it = m_goods.begin(); it != m_goods.end(); it++)
        addGoods(it.value());
    db.closeDB();
}

void SC_MonthlyCheck::aNew()
{
    int code = getIdByBarcode();
    if (!code) {
        QMessageBox::critical(this, tr("Error"), tr("Code not found"));
        return;
    }
    Goods *g = m_goods[code];
    if (!g) {
        QMessageBox::critical(this, tr("Error"), tr("Code exists, but not goods code"));
        return;
    }
    t->setCurrentCell(g->row, 0);
    double qty = QInputDialog::getDouble(this, tr("Qty"), tr("Qty"), 0, -1000, 10000, 3);
    g->setQty(QTY_FACT, g->qty[QTY_FACT] + qty);
}

void SC_MonthlyCheck::aEdit()
{
    QModelIndexList l = t->selectionModel()->selectedRows();
    if (!l.count())
        return;
    for (QMap<int, Goods*>::iterator it = m_goods.begin(); it != m_goods.end(); it++) {
        if (!it.value())
            continue;
        if (it.value()->row == l.at(0).row()) {
            double qty = QInputDialog::getDouble(this, tr("Qty"), tr("Qty"), 0, -1000, 10000, 3);
            it.value()->setQty(QTY_FACT, it.value()->qty[QTY_FACT] + qty);
            return;
        }
    }
}

QString SC_MonthlyCheck::title()
{
    return tr("Monthly check");
}

void SC_MonthlyCheck::updateRow()
{
    Goods *g = static_cast<Goods*>(sender());
    if (!t->item(g->row, 0))
        return;
    t->item(g->row, 8)->setData(Qt::EditRole, g->qty[QTY_DIFF]);
    t->item(g->row, 7)->setData(Qt::EditRole, g->qty[QTY_FACT]);
    t->setCurrentCell(g->row, 0);
    if (!db.openDB())
        return;
    if (!db.prepare("select id from stb where fgoods=:fgoods and fdoc=:fdoc"))
        return;
    db.bindValue(":fgoods", g->code);
    db.bindValue(":fdoc", m_docId);
    if (!db.execSQL())
        return;
    if (db.next()) {
        int rowId = db.v_int(0);
        if (!db.prepare("update stb set fqty=:fqty where id=:id"))
            return;
        db.bindValue(":fqty", g->qty[QTY_FACT]);
        db.bindValue(":id", rowId);
        if (!db.execSQL())
            return;
    } else {
        if (!db.prepare("insert into stb (fdoc, fgoods, fqty, fprice) values (:fdoc, :fgoods, :fqty, :fprice)"))
            return;
        db.bindValue(":fdoc", m_docId);
        db.bindValue(":fgoods", g->code);
        db.bindValue(":fqty", g->qty[QTY_FACT]);
        db.bindValue(":fprice", 0);
        if (!db.execSQL())
            return;
    }
    db.closeDB();
}

void Goods::setQty(int field, double q)
{
    qty[field] = q;
    qty[QTY_STORE] = qty[QTY_TILL] + qty[QTY_IN] - qty[QTY_OUT];
    qty[QTY_DIFF] = qty[QTY_FACT] - qty[QTY_STORE];
    emit qtyChanged();
}
