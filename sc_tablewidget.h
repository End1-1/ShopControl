#ifndef SC_TABLEWIDGET_H
#define SC_TABLEWIDGET_H

#include <QTableWidget>
#include <QItemDelegate>
#include "dbdriver.h"

class GridItemDelegate : public QItemDelegate {
public:
    QList<int> m_totals;
protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

class SC_TableWidget : public QTableWidget
{
    Q_OBJECT

public:
    typedef struct {
        int index;
        QString fieldName;
        QString caption;
        int width;
        bool countTotal;
        double total;
    } GridColumn;
    QMap<QString, GridColumn> m_columns;
    QMap<int, GridColumn> m_columnsIndex;
    SC_TableWidget(QWidget *parent = 0);
    void setColumn(const QString &fieldName, const QString &title, int width, bool countTotal = false, double total =0);
    void fetchColumns(QSqlRecord r);
    void fetchRow(DbDriver *d);
    void fetchRows(DbDriver *d);
    int selectedRows(QList<int> &list);
    QVariant v(const QString &fieldName, int row);
    int v_int(const QString &fieldName, int row);
    double v_dbl(const QString &fieldName, int row);
    void removeRow(int row);
signals:
    void totalChanged(int col, double value);
};

#endif // SC_TABLEWIDGET_H
