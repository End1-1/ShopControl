#ifndef SC_GRID_H
#define SC_GRID_H

#include <QMainWindow>
#include "dbdriver.h"
#include "sc_tablewidget.h"
#include "dx_tablefilter.h"
#include <QMessageBox>
#include <QToolBar>

namespace Ui {
class SC_Grid;
}

class SC_Grid : public QMainWindow
{
    Q_OBJECT

public:
    explicit SC_Grid(QWidget *parent = 0);
    ~SC_Grid();
    virtual QString title();
    QToolBar *toolBar();
protected:
    DbDriver db;
    QMap<QString, double> m_totalField;
    QString m_sqlQuery;
    QList<DX_TableFilter*> m_filterList;
    QMap<QString, DX_TableFilter*> m_filterMap;
    void openQuery();
    SC_TableWidget *t;
    DX_TableFilter *f(const QString &name);
    void setFilter(const QString &name, DX_TableFilter *filter);
    virtual void aNew();
    virtual void aEdit();
    virtual bool aDel();
    virtual void aFilter();
    virtual void aPrepareSql();
    virtual void aRefresh();
    virtual QVariant getId();
private slots:
    void on_actionNew_triggered();
    void on_actionFilter_triggered();
    void on_actionRemove_triggered();
    void on_actionRefresh_triggered();
    void slotTotalChanged(int col, double value);
    void on_actionEdit_triggered();
    void on_tblData_doubleClicked(const QModelIndex &index);

    void on_actionExcel_triggered();

private:
    Ui::SC_Grid *ui;

};

#endif // SC_GRID_H
