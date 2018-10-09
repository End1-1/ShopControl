#include "sc_grid.h"
#include "ui_sc_grid.h"
#include "sc_dlgfilter.h"
#include <QLibrary>
#include <QDir>

typedef int* (*Excel_Create)();
typedef int (*Excel_Show)(void*);
typedef int* (*Workbook_Add)(void *);
typedef int* (*Sheet_Select)(void *, int);
typedef int (*Sheet_Set_Cell_Value)(void*, int, int, const wchar_t *);
typedef int (*Sheet_Set_Col_Width)(void *, int, int);
typedef int (*Clear)(void*);
typedef int (*SaveToFile)(void*, const wchar_t *);

SC_Grid::SC_Grid(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SC_Grid)
{
    ui->setupUi(this);
    t = ui->tblData;
    connect(t, SIGNAL(totalChanged(int,double)), this, SLOT(slotTotalChanged(int,double)));
}

SC_Grid::~SC_Grid()
{
    delete ui;
}

QString SC_Grid::title()
{
    return tr("Title");
}

QToolBar *SC_Grid::toolBar()
{
    return ui->toolBar;
}

void SC_Grid::openQuery()
{
    if (!db.openDB())
        return;
    if (!db.execSQL(m_sqlQuery))
        return;
    t->clear();
    t->setRowCount(0);
    t->setSortingEnabled(false);
    t->fetchColumns(db.record());
    t->fetchRows(&db);
    ui->tblTotal->setColumnCount(t->columnCount());
    ui->tblTotal->setRowCount(1);
    for (int i = 0; i < ui->tblData->columnCount(); i++) {
        ui->tblTotal->setColumnWidth(i, ui->tblData->columnWidth(i));
    }
    QStringList header;
    header << QString::number(t->rowCount());
    ui->tblTotal->setVerticalHeaderLabels(header);
    for (QMap<QString, SC_TableWidget::GridColumn>::const_iterator it = t->m_columns.begin(); it != t->m_columns.end(); it++)
        if (it.value().countTotal) {
            ui->tblTotal->setItem(0, it.value().index, new QTableWidgetItem(QString::number(it.value().total, 'f', 0)));
        }
    db.closeDB();
    t->setSortingEnabled(true);
}

DX_TableFilter *SC_Grid::f(const QString &name)
{
    if (!m_filterMap.contains(name))
        return 0;
    return m_filterMap[name];
}

void SC_Grid::setFilter(const QString &name, DX_TableFilter *filter)
{
    m_filterMap[name] = filter;
    m_filterList.append(filter);
}

void SC_Grid::aNew()
{

}

void SC_Grid::aEdit()
{

}

bool SC_Grid::aDel()
{
    if (QMessageBox::warning(this, tr("Warning"), tr("Confirm deletion"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        return true;
    else
        return false;
}

void SC_Grid::aFilter()
{
    SC_DlgFilter *d = new SC_DlgFilter(m_filterList, this);
    if (d->exec() == QDialog::Accepted) {
        aPrepareSql();
        openQuery();
    }
    delete d;
}

void SC_Grid::aPrepareSql()
{

}

void SC_Grid::aRefresh()
{
    openQuery();
}

QVariant SC_Grid::getId()
{
    QModelIndexList sel = ui->tblData->selectionModel()->selectedRows();
    if (sel.count() == 0) {
        return QVariant(QVariant::Invalid);
    }
    return ui->tblData->item(sel.at(0).row(), 0)->data(Qt::DisplayRole);
}

void SC_Grid::on_actionNew_triggered()
{
    aNew();
}

void SC_Grid::on_actionFilter_triggered()
{
    aFilter();
}

void SC_Grid::on_actionRemove_triggered()
{
    aDel();
}

void SC_Grid::on_actionRefresh_triggered()
{
    aRefresh();
}

void SC_Grid::slotTotalChanged(int col, double value)
{
    ui->tblTotal->item(0, col)->setData(Qt::EditRole, value);
}

void SC_Grid::on_actionEdit_triggered()
{
    aEdit();
}

void SC_Grid::on_tblData_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    aEdit();
}

void SC_Grid::on_actionExcel_triggered()
{
    QLibrary lib(QDir::currentPath() + "/excel");
    if (!lib.load())
    {
        QMessageBox::critical(this, "Program error", "Could not load library " + QDir::currentPath() + "/excel.dll");
        return;
    }

    Excel_Create excel_create = (Excel_Create)(lib.resolve("Excel_Create"));
    Excel_Show excel_show = (Excel_Show)(lib.resolve("Excel_Show"));
    Workbook_Add workbook_add = (Workbook_Add)(lib.resolve("Workbook_Add"));
    Sheet_Select sheet_select = (Sheet_Select)(lib.resolve("Sheet_Select"));
    Sheet_Set_Cell_Value sheet_set_cell_value = (Sheet_Set_Cell_Value)(lib.resolve("Sheet_Set_Cell_Value"));
    Sheet_Set_Col_Width sheet_set_col_width = (Sheet_Set_Col_Width)(lib.resolve("Sheet_Set_Col_Width"));
    Clear clear = (Clear)(lib.resolve("Clear"));
    SaveToFile save = (SaveToFile)(lib.resolve("SaveToFile"));

    int *excel = excel_create();
    int *workbook = workbook_add(excel);
    int *sheet = sheet_select(workbook, 1);

    for (int i = 0; i < ui->tblTotal->columnCount(); i++)
    {
        sheet_set_cell_value(sheet, i + 1, 1, ui->tblData->m_columnsIndex[i].caption.toStdWString().c_str());
        sheet_set_col_width(sheet, i + 1, ui->tblData->columnWidth(i) / 7);
    }

    for (int i = 0; i < ui->tblData->rowCount(); i++)
        for (int j =  0; j < ui->tblData->columnCount(); j++)
            sheet_set_cell_value(sheet, j + 1, i + 2, ui->tblData->item(i, j)->text().toStdWString().c_str());

    int footerRow = ui->tblTotal->rowCount() + ui->tblData->rowCount() + 2;
    for (int i = 0; i < ui->tblData->rowCount(); i++)
        for (int j =  0; j < ui->tblTotal->columnCount(); j++) {
            QTableWidgetItem *item = ui->tblTotal->item(i, j);
            if (item)
                sheet_set_cell_value(sheet, j + 1, i + footerRow, item->text().toStdWString().c_str());
        }

    excel_show(excel);
    /*if (fileName.length())
        save(workbook, fileName.toStdWString().c_str()); */
    clear(sheet);
    clear(workbook);
    clear(excel);
}
