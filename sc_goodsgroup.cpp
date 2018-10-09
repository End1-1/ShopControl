#include "sc_goodsgroup.h"
#include "dlggoodsgroups.h"

SC_GoodsGroup::SC_GoodsGroup(QWidget *parent) :
    SC_Grid(parent)
{
    setWindowTitle(tr("Goods groups"));
    t->setColumn("ID", tr("Number"), 80);
    t->setColumn("FNAME", tr("Name"), 300);
    t->setColumn("ADGCODE", tr("ADGT"), 100);
    aPrepareSql();
    openQuery();
}

QString SC_GoodsGroup::title()
{
    return tr("Goods groups");
}

void SC_GoodsGroup::aNew()
{
    DlgGoodsGroups::group();
}

void SC_GoodsGroup::aEdit()
{
    int id = getId().toInt();
    if (id > 0) {
        DlgGoodsGroups::group(id);
    }
}

void SC_GoodsGroup::aPrepareSql()
{
    m_sqlQuery = "select id, fname, adgcode from groups order by fname";
}
