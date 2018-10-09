#ifndef DX_TABLEFILTER_H
#define DX_TABLEFILTER_H

#include <QObject>
#include <QPushButton>
#include <QMap>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QVariant>

class DX_TableFilter : public QObject
{
    Q_OBJECT
public:
    enum WidgetType {wtLineEdit, wtDateEdit};
    enum SQLCOND {IN, LIKE, EQUAL};

    explicit DX_TableFilter(WidgetType wt, const QString &title, bool checkBox, QWidget *parent = 0);
    ~DX_TableFilter();
    void addToLayout(QGridLayout *l);
    void rollback();
    void setValue(const QVariant &value);
    void setCache(const QString &cacheName);
    QDate getDate();
    QString getDateString();
    QVariant getValue();
    bool isIncluded();
    void setIncluded(bool value);
    bool sqlCond(QString &where, const QString &cond, SQLCOND s = IN);
    QString &sqlCondFinish(QString &where);

private:
    QWidget *m_parentWidget;
    QString m_cacheName;
    QString m_titleText;
    bool m_checkBoxValue;
    WidgetType m_widgetType;
    QWidget *m_widget;
    QLabel *m_title;
    QPushButton *m_btnCache;
    QCheckBox *m_checkBox;
    QMap<QString, int> m_data;

    QVariant m_value;
    QVariant m_tempValue;
    QString m_valueName;

private slots:
    void slotCheckboxValue(bool checked);
    void slotTextChanged(const QString &text);
    void slotDateChanged(const QDate &date);
    void slotGetValuesFromCache(bool v);
    void slotClearValues(bool v);
};

#endif // DX_TABLEFILTER_H
