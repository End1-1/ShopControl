#include "sc_docs.h"
#include "common.h"
#include "sc_storedoc.h"
#include <QDate>

SC_Docs::SC_Docs(QWidget *parent) :
    SC_Grid(parent)
{
    setWindowTitle(tr("Documents"));
    setFilter("type", new DX_TableFilter(DX_TableFilter::wtLineEdit, tr("Type"), true, this));
    setFilter("date1", new DX_TableFilter(DX_TableFilter::wtDateEdit, tr("Date"), true, this));
    setFilter("date2", new DX_TableFilter(DX_TableFilter::wtDateEdit, tr("Date"), false, this));
    setFilter("group", new DX_TableFilter(DX_TableFilter::wtLineEdit, tr("Group"), true, this));
    setFilter("flags", new DX_TableFilter(DX_TableFilter::wtLineEdit, tr("Flags"), false, this));
    f("date1")->setValue(QDate::currentDate());
    f("date2")->setValue(QDate::currentDate());
    f("type")->setValue(1);
    f("group")->setCache("GROUPS");
    f("flags")->setCache("ORDER_FLAGS");
    f("type")->setCache("ST_TYPE");
    t->setColumn("ID", tr("Order"), 80);
    t->setColumn("FDATE", tr("Date"), 120);
    t->setColumn("TYPE", tr("Type"), 100);
    t->setColumn("RECID", tr("Rec id"), 0);
    t->setColumn("GR_NAME", tr("Group"), 120);
    t->setColumn("G_NAME", tr("Goods"), 150);
    t->setColumn("FQTY", tr("Qty"), 100, true);
    t->setColumn("FPRICE", tr("Price"), 100);
    t->setColumn("AMOUNT", tr("Amount"), 120, true);
    aPrepareSql();
    openQuery();
}

void SC_Docs::aPrepareSql()
{
    QString select = "select sth.id, stb.id as recid ";
    if (f("date1")->isIncluded()) {
        select += ",sth.fdate ";
    }
    select += ", gr.fname as gr_name, g.fname as g_name, "
            "stb.fqty, stb.fprice, stb.fqty*stb.fprice as amount ";
    QString where = "where sth.id=stb.fdoc and stb.fgoods=g.id and g.fgroup=gr.id and sth.fdate between "
            + f("date1")->getDateString() + " and " + f("date2")->getDateString() + " and";
    QString from = "from sth, stb, groups gr, goods g ";
    f("group")->sqlCond(where, "gr.id");
    if (f("flags")->sqlCond(where, "f.fflag")) {
        from += ",stf f ";
        where += " sth.id=f.forder and";
    }
    if (f("type")->sqlCond(where, "sth.ftype") || f("type")->isIncluded()) {
        from += ",st_type st ";
        where += " sth.ftype=st.id and";
    }
    if (f("type")->isIncluded()) {
        select += ",st.fname as type ";
    }

    where = f("group")->sqlCondFinish(where);
    m_sqlQuery = select + from + where;
}

bool SC_Docs::aDel()
{
    if (!SC_Grid::aDel())
        return false;
    QList<int> rows;
    if (!t->selectedRows(rows)) {
        QMessageBox::warning(this, tr("Warning"), tr("Nothing is selected"));
        return false;
    }

    if (!db.openDB())
        return false;
    for (int i = rows.count() - 1; i > -1; i--) {
        db.prepare("delete from stb where id=:id");
        db.bindValue(":id", t->v_int("RECID", rows.at(i)));
        db.execSQL();
        db.prepare("update sth set famount=(select sum(fqty*fprice) from stb where fdoc=:fdoc) where id=:id");
        db.bindValue(":fdoc", t->v_int("ID", rows.at(i)));
        db.bindValue(":id", t->v_int("ID", rows.at(i)));
        db.execSQL();
        t->removeRow(rows.at(i));
    }
    db.execSQL("delete from sth where famount=0");
    db.closeDB();
    return true;
}

void SC_Docs::aNew()
{
    fMainWindow->createWindow<SC_StoreDoc>();
}

QString SC_Docs::title()
{
    return tr("Documents");
}
