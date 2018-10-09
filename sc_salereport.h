#ifndef SC_SALEREPORT_H
#define SC_SALEREPORT_H

#include "sc_grid.h"

class SC_SaleReport : public SC_Grid
{
public:
    SC_SaleReport(QWidget *parent = 0);
    virtual void aPrepareSql();
    virtual QString title();
};

#endif // SC_SALEREPORT_H
