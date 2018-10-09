#ifndef SC_GOODS_H
#define SC_GOODS_H

#include "sc_grid.h"

class SC_Goods : public SC_Grid
{
    Q_OBJECT

public:
    SC_Goods(QWidget *parent = 0);
    virtual QString title();
protected:
    virtual void aNew();
    virtual void aEdit();
    virtual void aPrepareSql();
    virtual bool aDel();
};

#endif // SC_GOODS_H
