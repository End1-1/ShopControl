#include "sc_dialog.h"
#include <QKeyEvent>

SC_Dialog::SC_Dialog(QWidget *parent) :
    QDialog(parent)
{
    installEventFilter(this);
}

bool SC_Dialog::eventFilter(QObject *o, QEvent *e)
{
    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *k = static_cast<QKeyEvent*>(e);
        switch (k->key()) {
        case Qt::Key_Escape:
        case Qt::Key_Enter:
        case Qt::Key_Return:
            return true;
        }
    }
    return QDialog::eventFilter(o, e);
}

