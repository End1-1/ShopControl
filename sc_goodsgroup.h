#ifndef SC_GOODSGROUP_H
#define SC_GOODSGROUP_H

#include "sc_grid.h"

class SC_GoodsGroup : public SC_Grid
{
    Q_OBJECT
public:
    SC_GoodsGroup(QWidget *parent = 0);
    virtual QString title();
    virtual void aNew();
    virtual void aEdit();
    virtual void aPrepareSql();
};

#endif // SC_GOODSGROUP_H
