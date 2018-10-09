#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sc_toolbutton.h"
#include <QMainWindow>
#include <QMdiArea>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    template<class T>
    T *createWindow() {        
        T *t = new T(this);
        static_cast<QMainWindow*>(t)->showMaximized();
        m_tabWidget->addTab(t, t->title());
        m_tabWidget->setCurrentIndex(m_tabWidget->count() - 1);
        return t;
    }

private slots:
    void tabCloseRequest(int index);
    void on_actionGoods_triggered();

    void on_actionDocuments_triggered();

    void on_actionLogin_triggered();

    void on_actionLogs_triggered();

    void on_actionExit_triggered();

    void on_actionMonthly_check_triggered();

    void on_actionNew_store_document_triggered();

    void on_actionChange_password_triggered();

    void on_actionInput_documents_triggered();

    void on_actionStore_facts_triggered();

    void on_actionPartners_triggered();

    void on_actionSale_report_triggered();

    void on_actionCurrent_store_triggered();

    void on_actionChange_sale_triggered();

    void on_actionSettings_triggered();

    void on_actionGoods_groups_triggered();

private:
    Ui::MainWindow *ui;
    QTabWidget *m_tabWidget;
    void enableall(bool v);
};

#endif // MAINWINDOW_H
