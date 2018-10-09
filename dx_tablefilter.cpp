#include "dx_tablefilter.h"
#include <QLineEdit>
#include <QDateEdit>
#include <QComboBox>
#include "dx_dlgcache.h"

DX_TableFilter::DX_TableFilter(WidgetType wt, const QString &title, bool checkBox, QWidget *parent) :
    QObject(parent)
{
    m_widgetType = wt;
    m_titleText = title;
    m_checkBoxValue = checkBox;
    m_btnCache = 0;
    m_checkBox = 0;
    m_checkBoxValue = false;
    if (checkBox) {
        m_checkBox = new QCheckBox(0);
        delete m_checkBox;
    }
}

DX_TableFilter::~DX_TableFilter()
{
    m_widget = 0;
}

void DX_TableFilter::addToLayout(QGridLayout *l)
{
    m_parentWidget = qobject_cast<QWidget*>(l->parent());
    m_title = new QLabel(m_titleText, m_parentWidget);
    switch (m_widgetType) {
    case wtLineEdit: {
        m_widget = new QLineEdit(m_parentWidget);
        QLineEdit *o = qobject_cast<QLineEdit*>(m_widget);
        o->setText(m_value.toString());
        connect(o, SIGNAL(textChanged(QString)), this, SLOT(slotTextChanged(QString)));
        break;
    }
    case wtDateEdit: {
        m_widget = new QDateEdit(m_parentWidget);
        QDateEdit *o = qobject_cast<QDateEdit*>(m_widget);
        connect(o, SIGNAL(dateChanged(QDate)), this, SLOT(slotDateChanged(QDate)));
        o->setDate(m_value.toDate());
        o->setCalendarPopup(true);
        break;
    }
    }


    int row = l->rowCount();
    l->addWidget(m_title, row, 0);
    l->addWidget(m_widget, row, 1);

    if (m_checkBox) {
        m_checkBox = new QCheckBox(m_parentWidget);
        m_checkBox->setParent(qobject_cast<QWidget*>(m_parentWidget));
        m_checkBox->setChecked(m_checkBoxValue);
        connect(m_checkBox, SIGNAL(clicked(bool)), this, SLOT(slotCheckboxValue(bool)));
        l->addWidget(m_checkBox, row, 2);
    }

    if (m_cacheName.length()) {
        m_btnCache = new QPushButton("+", m_parentWidget);
        m_btnCache->setMaximumSize(20, 20);
        m_btnCache->setMinimumSize(20, 20);
        connect(m_btnCache, SIGNAL(clicked(bool)), this, SLOT(slotGetValuesFromCache(bool)));
        l->addWidget(m_btnCache, row, 3);
    }

    if (m_widgetType == wtLineEdit) {
        QPushButton *btnClear = new QPushButton("-", m_parentWidget);
        btnClear->setMaximumSize(20, 20);
        btnClear->setMinimumSize(20, 20);
        connect(btnClear, SIGNAL(clicked(bool)), this, SLOT(slotClearValues(bool)));
        l->addWidget(btnClear, row, 4);
    }

    m_tempValue = m_value;
}

void DX_TableFilter::rollback()
{
    m_value = m_tempValue;
}

void DX_TableFilter::setValue(const QVariant &value)
{
    m_value = value;
}

void DX_TableFilter::setCache(const QString &cacheName)
{
    m_cacheName = cacheName;
}

QDate DX_TableFilter::getDate()
{
    return m_value.toDate();
}

QString DX_TableFilter::getDateString()
{
    return "'" + getDate().toString("yyyy-MM-dd") + "'";
}

QVariant DX_TableFilter::getValue()
{
    return m_value;
}

bool DX_TableFilter::isIncluded()
{
    return m_checkBoxValue;
}

void DX_TableFilter::setIncluded(bool value)
{
    m_checkBoxValue = value;
}

bool DX_TableFilter::sqlCond(QString &where, const QString &cond, SQLCOND s)
{
    if (!m_value.toString().length())
        return false;
    switch (s) {
    case IN:
        where += " " + cond + " in (" + m_value.toString() + ") and";
        break;
    case LIKE:
        where += " " + cond + " LIKE '%" + m_value.toString() + "%' and";
        break;
    case EQUAL:
        where += " " + cond + "=" + m_value.toString() + " and";
        break;
    }
    return true;
}

QString &DX_TableFilter::sqlCondFinish(QString &where)
{
    where = where.remove(where.length() - 4, 4);
    return where;
}

void DX_TableFilter::slotCheckboxValue(bool checked)
{
    m_checkBoxValue = checked;
}

void DX_TableFilter::slotTextChanged(const QString &text)
{
    m_value = text;
}

void DX_TableFilter::slotDateChanged(const QDate &date)
{
    m_value = date;
}

void DX_TableFilter::slotGetValuesFromCache(bool v)
{
    Q_UNUSED(v)
    DX_DlgCache *d = new DX_DlgCache(m_valueName, m_cacheName, m_parentWidget);
    if (d->exec() == QDialog::Accepted) {
        m_tempValue = d->valueId;
        m_valueName = d->valueName;
        if (m_widget) {
            switch (m_widgetType) {
            case wtLineEdit:
                qobject_cast<QLineEdit*>(m_widget)->setText(m_tempValue.toString());
                break;
            case wtDateEdit:
                qobject_cast<QDateEdit*>(m_widget)->setDate(m_tempValue.toDate());
                break;
            }
        }
    }
    delete d;
}

void DX_TableFilter::slotClearValues(bool v)
{
    Q_UNUSED(v)
    m_value = "";
    qobject_cast<QLineEdit*>(m_widget)->clear();
}
