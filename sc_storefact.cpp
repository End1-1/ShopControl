#include "sc_storefact.h"

SC_StoreFact::SC_StoreFact(QWidget *parent) :
    SC_Grid(parent)
{
    setWindowTitle(tr("Facts"));
    t->setColumn("ID", tr("Number"), 80);
    t->setColumn("FDATE", tr("Date"), 100);
    t->setColumn("FAMOUNT", tr("Amount"), 150, true);
    aPrepareSql();
    openQuery();
}

void SC_StoreFact::aPrepareSql()
{
    QString sql = QString("select d.id, d.fdate, d.famount "
            "from sth d "
            "where d.ftype=4 ORDER BY d.fdate desc ");
    m_sqlQuery = sql;
}

QString SC_StoreFact::title()
{
    return tr("Facts");
}
