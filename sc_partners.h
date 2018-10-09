#ifndef SC_PARTNERS_H
#define SC_PARTNERS_H

#include "sc_grid.h"

class SC_Partners : public SC_Grid
{
    Q_OBJECT
public:
    SC_Partners(QWidget *parent = 0);
    virtual void aPrepareSql();
    virtual void aNew();
    virtual void aEdit();
    virtual QString title();
};

#endif // SC_PARTNERS_H
