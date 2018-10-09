#ifndef SC_STOREFACT_H
#define SC_STOREFACT_H

#include "sc_grid.h"

class SC_StoreFact : public SC_Grid
{
    Q_OBJECT
public:
    SC_StoreFact(QWidget *parent = 0);
    virtual void aPrepareSql();
    virtual QString title();
};

#endif // SC_STOREFACT_H
