#include "sc_tablewidget.h"
#include <QSet>
#include <QPainter>

SC_TableWidget::SC_TableWidget(QWidget *parent) :
    QTableWidget(parent)
{
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setItemDelegate(new GridItemDelegate());
}

void SC_TableWidget::setColumn(const QString &fieldName, const QString &title, int width, bool countTotal, double total)
{
    GridColumn g;
    g.fieldName = fieldName;
    g.caption = title;
    g.width = width;
    g.countTotal = countTotal;
    g.total = total;
    m_columns[fieldName] = g;
}

void SC_TableWidget::fetchColumns(QSqlRecord r)
{
    setColumnCount(r.count());
    setColumnCount(r.count());
    QStringList header;
    for (int i = 0; i < r.count(); i++) {
        if (m_columns.contains(r.fieldName(i))) {
            GridColumn &g = m_columns[r.fieldName(i)];
            header << g.caption;
            setColumnWidth(i, g.width);
            g.total = 0;
            g.index = i;
            m_columnsIndex[i] = g;
        }
        else {
            GridColumn g;
            g.index = i;
            g.fieldName = r.fieldName(i);
            g.caption = g.fieldName;
            g.width = 100;
            g.countTotal = false;
            g.total = 0;
            m_columns[g.fieldName] = g;
            m_columnsIndex[i] = g;
            header << r.fieldName(i);
            setColumnWidth(i, 100);
        }
    }
    setHorizontalHeaderLabels(header);
}

void SC_TableWidget::fetchRow(DbDriver *db)
{
    int row = rowCount();
    setRowCount(row + 1);
    for (int i = 0; i < columnCount(); i++) {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setData(Qt::EditRole, db->v(i));
        setItem(row, i, item);
        if (m_columnsIndex[i].countTotal)
            m_columnsIndex[i].total += db->v_dbl(i);
    }
}

void SC_TableWidget::fetchRows(DbDriver *d)
{
    while (d->next())
        fetchRow(d);
    for (QMap<QString, GridColumn>::iterator it = m_columns.begin(); it != m_columns.end(); it++)
        it.value().total = m_columnsIndex[it.value().index].total;
}

int SC_TableWidget::selectedRows(QList<int> &list)
{
    QSet<int> r;
    QModelIndexList l = selectedIndexes();
    for (QModelIndexList::const_iterator it = l.begin(); it != l.end(); it++)
        r.insert(it->row());
    list = r.toList();
    qSort(list);
    return list.count();
}

QVariant SC_TableWidget::v(const QString &fieldName, int row)
{
    int col = m_columns[fieldName].index;
    return item(row, col)->data(Qt::EditRole);
}

int SC_TableWidget::v_int(const QString &fieldName, int row)
{
    return v(fieldName, row).toInt();
}

double SC_TableWidget::v_dbl(const QString &fieldName, int row)
{
    return v(fieldName, row).toDouble();
}

void SC_TableWidget::removeRow(int row)
{
    for (QMap<QString, GridColumn>::iterator it = m_columns.begin(); it != m_columns.end(); it++) {
        if (it.value().countTotal) {
            it.value().total -= v_dbl(it.value().fieldName, row);
            emit totalChanged(it.value().index, it.value().total);
        }
    }
    QTableWidget::removeRow(row);
}


void GridItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (!index.isValid())
        return;

    painter->save();

    QString value;
    switch (index.data(Qt::EditRole).type()) {
    case QVariant::String:
        value = index.data(Qt::EditRole).toString();
        break;
    case QVariant::Int:
        value = QString::number(index.data(Qt::EditRole).toInt());
        break;
    case QVariant::Double: {
        QLocale l(QLocale::English, QLocale::UnitedStates);
        value = l.toString(index.data(Qt::EditRole).toDouble(), 'f', 2);
        break;
    }
    case QVariant::Date:
        value = index.data(Qt::EditRole).toDate().toString("dd.MM.yyyy");
        break;
    case QVariant::DateTime:
        value = index.data(Qt::EditRole).toDateTime().toString("dd.MM.yyyy HH:mm");
        break;
    default:
        value = "";
        break;
    }


    QBrush brush(Qt::white);
    if (option.state & QStyle::State_Selected)
        brush.setColor(QColor::fromRgb(166, 217, 244));
    painter->setBrush(brush);
    painter->fillRect(option.rect, brush);

    QRect textRect = option.rect;
    textRect.adjust(2, 2, 2, 2);
    painter->drawText(textRect, value);

    painter->restore();
}
