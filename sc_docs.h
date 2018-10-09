#ifndef SC_DOCS_H
#define SC_DOCS_H

#include "sc_grid.h"

class SC_Docs : public SC_Grid
{
    Q_OBJECT
public:
    SC_Docs(QWidget *parent = 0);
    virtual void aPrepareSql();
    virtual bool aDel();
    virtual void aNew();
    virtual QString title();
};

#endif // SC_DOCS_H
