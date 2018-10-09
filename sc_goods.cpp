#include "sc_goods.h"
#include "sc_editgoods.h"

SC_Goods::SC_Goods(QWidget *parent) :
    SC_Grid(parent)
{
    t->setColumn("ID", tr("Id"), 80);
    t->setColumn("FSTATE", tr("State code"), 0);
    t->setColumn("GS_NAME", tr("State"), 100);
    t->setColumn("FGROUP", tr("Group code"), 0);
    t->setColumn("GG_NAME", tr("Group"), 150);
    t->setColumn("FNAME", tr("Name"), 200);
    t->setColumn("FPRICE", tr("Sale price"), 120);
    t->setColumn("FPRICE2", tr("Buy price"), 120);
    setFilter("group", new DX_TableFilter(DX_TableFilter::wtLineEdit, tr("Group"), false, this));
    f("group")->setCache("GROUPS");
    aPrepareSql();
    openQuery();
}

void SC_Goods::aNew()
{
    SC_EditGoods *d = new SC_EditGoods(this);
    d->exec();
    delete d;
}

void SC_Goods::aEdit()
{
    QList<int> rows;
    if (!t->selectedRows(rows)) {
        QMessageBox::warning(this, tr("Warning"), tr("Nothing is selected"));
        return;
    }
    SC_EditGoods *d = new SC_EditGoods(this);
    d->load(t->v_int("id", rows.at(0)));
    d->exec();
    delete d;
}

void SC_Goods::aPrepareSql()
{
    QString select = "select g.id, fstate, gs.fname as gs_name, fgroup, gg.fname as gg_name, g.fname, fprice, fprice2 "
                 "from goods g, groups gg, goods_state gs ";
    QString where = "where g.fgroup=gg.id and g.fstate=gs.id and";
    f("group")->sqlCond(where, "gg.id");
    where = f("group")->sqlCondFinish(where);
    m_sqlQuery = select + where + " order by gg_name, fname";
}

bool SC_Goods::aDel()
{
    int id = getId().toInt();
    if (!id) {
        return false;
    }
    db.prepare("select id from stb where fgoods=:fgoods");
    db.bindValue(":fgoods", id);
    db.execSQL();
    if (db.next()) {
        db.closeDB();
        QMessageBox::warning(this, tr("Error"), tr("This item in use, you cannot delete this"));
        return false;
    }
    if (QMessageBox::question(this, tr("Remove goods"), tr("Confirm to remove the selected goods"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return false;
    }
    db.prepare("delete from goods where id=:id");
    db.bindValue(":id", id);
    db.execSQL();
    db.closeDB();
    return true;
}

QString SC_Goods::title()
{
    return tr("Goods");
}
