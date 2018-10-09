#include "sc_partners.h"
#include "dlgpartner.h"

SC_Partners::SC_Partners(QWidget *parent) :
    SC_Grid(parent)
{
    setWindowTitle(tr("Partners"));
    t->setColumn("ID", tr("Number"), 80);
    t->setColumn("HVHH", tr("Tax number"), 100);
    t->setColumn("LLC", tr("Name"), 300);
    t->setColumn("ADDITIONAL", tr("Additional"), 300, true);
    aPrepareSql();
    openQuery();
}

void SC_Partners::aPrepareSql()
{
    m_sqlQuery = "select id, hvhh, llc, additional from partners";
}

void SC_Partners::aNew()
{
    DlgPartner::edit(0);
}

void SC_Partners::aEdit()
{
    int id = getId().toInt();
    if (id > 0) {
        DlgPartner::edit(id);
    }
}

QString SC_Partners::title()
{
    return tr("Partners");
}
