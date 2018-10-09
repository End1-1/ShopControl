#include "sc_salereport.h"

SC_SaleReport::SC_SaleReport(QWidget *parent) :
    SC_Grid(parent)
{
    setWindowTitle(tr("Sales report"));
    setFilter("date1", new DX_TableFilter(DX_TableFilter::wtDateEdit, tr("Date"), true, this));
    setFilter("date2", new DX_TableFilter(DX_TableFilter::wtDateEdit, tr("Date"), false, this));
    setFilter("partner", new DX_TableFilter(DX_TableFilter::wtLineEdit, tr("Group"), true, this));
    f("date1")->setValue(QDate::currentDate());
    f("date2")->setValue(QDate::currentDate());
    t->setColumn("FGROUP", tr("Group"), 150);
    t->setColumn("FGOODS", tr("Name"), 300);
    t->setColumn("FQTY", tr("Qty"), 80, true);
    t->setColumn("FAMOUNTSALE", tr("Amount, sale"), 150, true);
    t->setColumn("FAMOUNTBUY", tr("Amount, buy"), 150, true);
    aPrepareSql();
    openQuery();
}

void SC_SaleReport::aPrepareSql()
{
    m_sqlQuery = QString("select gg.fname as fgroup, g.fname as fgoods, sum(b.fqty) as fqty, \
    sum(b.fqty*b.fprice) as famountsale, sum(b.fqty*g.fprice2) as famountbuy \
    from stb b \
    left join sth h on h.id=b.fdoc \
    left join goods g on g.id=b.fgoods \
    left join groups gg on gg.id=g.fgroup \
    where h.fdate between %1 and %2 and h.ftype=1 \
    group by 1, 2 \
    having sum(b.fqty)<>0 \
    order by 1, 2")
                         .arg(f("date1")->getDateString())
                         .arg(f("date2")->getDateString());
}

QString SC_SaleReport::title()
{
    return tr("Sales report");
}

