#include "sc_dlgfilter.h"
#include <QHBoxLayout>

SC_DlgFilter::SC_DlgFilter(QList<DX_TableFilter*> &filter, QWidget *parent) :
    SC_Dialog(parent),
    m_filter(filter)
{
    setWindowTitle(parent->windowTitle());
    setMinimumWidth(500);
    m_layout = new QGridLayout(this);

    for (QList<DX_TableFilter*>::iterator it = filter.begin(); it != filter.end(); it++)
        (*it)->addToLayout(m_layout);

    QWidget *w = new QWidget(this);
    QHBoxLayout *l = new QHBoxLayout(w);
    QPushButton *btnOk = new QPushButton(tr("OK"), this);
    QPushButton *btnCancel = new QPushButton(tr("Cancel"), this);
    connect(btnOk, SIGNAL(clicked(bool)), this, SLOT(ok(bool)));
    connect(btnCancel, SIGNAL(clicked(bool)), this, SLOT(cancel(bool)));
    l->addStretch();
    l->addWidget(btnOk);
    l->addWidget(btnCancel);
    l->addStretch();
    w->setLayout(l);
    m_layout->addWidget(w, m_layout->rowCount(), 0, 1, 4);

    setLayout(m_layout);
}

void SC_DlgFilter::ok(bool v)
{
    Q_UNUSED(v)
    accept();
}

void SC_DlgFilter::cancel(bool v)
{
    Q_UNUSED(v)
    for (QList<DX_TableFilter*>::iterator it = m_filter.begin(); it != m_filter.end(); it++)
        (*it)->rollback();
    reject();
}

