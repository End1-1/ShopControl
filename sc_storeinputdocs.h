#ifndef SC_STOREINPUTDOCS_H
#define SC_STOREINPUTDOCS_H

#include "sc_grid.h"

class SC_StoreInputDocs : public SC_Grid
{
    Q_OBJECT
public:
    SC_StoreInputDocs(QWidget *parent = 0);
    virtual void aPrepareSql();
    virtual void aNew();
    virtual void aEdit();
    virtual bool aDel();
    void setFirstDate(const QDate &date);
    QString fAnd;
};

#endif // SC_STOREINPUTDOCS_H
