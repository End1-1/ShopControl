#ifndef SC_CURRENTSTORE_H
#define SC_CURRENTSTORE_H

#include "sc_grid.h"

class SC_CurrentStore : public SC_Grid
{
    Q_OBJECT
public:
    SC_CurrentStore(QWidget *parent = 0);
    virtual void aPrepareSql();
    virtual void aNew();
    virtual void aEdit();
    virtual bool aDel();
    virtual QString title();
};

#endif // SC_CURRENTSTORE_H
