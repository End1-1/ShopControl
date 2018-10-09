#ifndef SC_DLGFILTER_H
#define SC_DLGFILTER_H

#include "sc_dialog.h"
#include "dx_tablefilter.h"
#include <QGridLayout>

class SC_DlgFilter : public SC_Dialog
{
private:
    Q_OBJECT
    QList<DX_TableFilter*> &m_filter;
    QGridLayout *m_layout;
public:
    SC_DlgFilter(QList<DX_TableFilter*> &filter, QWidget *parent = 0);
private slots:
    void ok(bool v);
    void cancel(bool v);
};

#endif // SC_DLGFILTER_H
