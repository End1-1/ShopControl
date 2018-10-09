#include "sc_currentstore.h"
#include "sc_storeinputdocs.h"
#include "common.h"

SC_CurrentStore::SC_CurrentStore(QWidget *parent) :
    SC_Grid(parent)
{
    setWindowTitle(tr("Materials in store"));
    t->setColumn("GID", tr("Goods"), 0);
    t->setColumn("FGROUP", tr("Group"), 150);
    t->setColumn("FGOODS", tr("Name"), 300);
    t->setColumn("QTY", tr("Qty"), 80, true);
    t->setColumn("FAMOUNTSALE", tr("Amount, sale"), 150, true);
    t->setColumn("FAMOUNTBUY", tr("Amount, buy"), 150, true);
    setFilter("group", new DX_TableFilter(DX_TableFilter::wtLineEdit, tr("Group"), false, this));
    f("group")->setCache("GROUPS");
    aFilter();
}

void SC_CurrentStore::aPrepareSql()
{
    QString group;
    if (!f("group")->getValue().toString().isEmpty()) {
        group = " and g.fgroup in (" + f("group")->getValue().toString() + ") ";
    }
    m_sqlQuery = "select g.id, gg.fname as fgroup, g.fname as fgoods, coalesce(i.qty, 0)-coalesce(o.qty, 0) as qty, \
            (coalesce(i.qty, 0)-coalesce(o.qty, 0))*g.fprice as famountsale, (coalesce(i.qty, 0)-coalesce(o.qty, 0)) * g.fprice2 as famountbuy \
            from goods g \
            left join groups gg on gg.id=g.fgroup \
            left join (select bb.fgoods, sum(fqty) as qty from stb bb left join sth hh on hh.id=bb.fdoc where hh.ftype=1 group by 1) o on o.fgoods=g.id \
            left join (select bb.fgoods, sum(fqty) as qty from stb bb left join sth hh on hh.id=bb.fdoc where hh.ftype=2 group by 1) i on i.fgoods=g.id \
            where coalesce(i.qty, 0)-coalesce(o.qty, 0) <> 0 "  + group + " \
            order by 1, 2";
}

void SC_CurrentStore::aNew()
{

}

void SC_CurrentStore::aEdit()
{
    if (getId().toInt() == 0) {
        return;
    }
    SC_StoreInputDocs *s = fMainWindow->createWindow<SC_StoreInputDocs>();
    s->fAnd = " and d.id in (select fdoc from stb where fgoods=" + getId().toString() + ")";
    s->setFirstDate(QDate::fromString("01.01.2017", "dd.MM.yyyy"));
}

bool SC_CurrentStore::aDel()
{
    int id = getId().toInt();
    if (!id) {
        return false;
    }
    if (QMessageBox::warning(this, tr("Remove"), tr("Are sure to remove all sales?"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return false;
    }
    db.openDB();
    db.prepare("delete from stb where fgoods=:fgoods");
    db.bindValue(":fgoods", id);
    db.execSQL();
    db.closeDB();
    return true;
}

QString SC_CurrentStore::title()
{
    return tr("Materials in store");
}
