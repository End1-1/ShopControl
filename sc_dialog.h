#ifndef SC_DIALOG_H
#define SC_DIALOG_H

#include <QDialog>
#include "dbdriver.h"
#include <QEvent>
#include <QMessageBox>

class SC_Dialog : public QDialog
{
    Q_OBJECT

public:
    SC_Dialog(QWidget *parent = 0);

protected:
    DbDriver db;
    virtual bool eventFilter(QObject *o, QEvent *e);
};

#endif // SC_DIALOG_H
