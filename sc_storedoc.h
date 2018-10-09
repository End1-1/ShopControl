#ifndef SC_STOREDOC_H
#define SC_STOREDOC_H

#include <QMainWindow>
#include "dbdriver.h"

namespace Ui {
class SC_StoreDoc;
}

class SC_StoreDoc : public QMainWindow
{
    Q_OBJECT

public:
    explicit SC_StoreDoc(QWidget *parent = 0);
    ~SC_StoreDoc();
    void load(int id);
    virtual QString title();
private slots:
    void on_actionBarcode_triggered();
    void on_tblData_doubleClicked(const QModelIndex &index);
    void on_actionSave_triggered();
    void on_actionDelete_row_triggered();

private:
    DbDriver db;
    int m_id;
    Ui::SC_StoreDoc *ui;
    void setData(int row);
    void countAmount();
};

#endif // SC_STOREDOC_H
