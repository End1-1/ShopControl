#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sc_goods.h"
#include "sc_partners.h"
#include "sc_docs.h"
#include "sc_storefact.h"
#include "sc_login.h"
#include "sc_log.h"
#include "sc_monthlycheck.h"
#include "sc_storedoc.h"
#include "dlgchangepass.h"
#include "sc_storeinputdocs.h"
#include "sc_salereport.h"
#include "common.h"
#include "sc_currentstore.h"
#include "dlgchangesale.h"
#include "dlgsettings.h"
#include "sc_goodsgroup.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showMaximized();
    enableall(false);
    qApp->processEvents();
    on_actionLogin_triggered();
    fMainWindow = this;
    m_tabWidget = ui->tabw;
    m_tabWidget->setTabsClosable(true);
    connect(m_tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(tabCloseRequest(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tabCloseRequest(int index)
{
    QWidget *w = ui->tabw->widget(index);
    delete w;
    //ui->tabw->removeTab(index);
}

void MainWindow::on_actionGoods_triggered()
{
    createWindow<SC_Goods>();
}

void MainWindow::on_actionDocuments_triggered()
{
    createWindow<SC_Docs>();
}

void MainWindow::on_actionLogin_triggered()
{
    SC_Login *d = new SC_Login(this);
    if (d->exec() == QDialog::Accepted)
        enableall(true);
    delete d;
}

void MainWindow::enableall(bool v)
{
    ui->menuReport->setEnabled(v);
    ui->menuIndexes->setEnabled(v);
}

void MainWindow::on_actionLogs_triggered()
{
    createWindow<SC_Log>();
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionMonthly_check_triggered()
{
    createWindow<SC_MonthlyCheck>();
}

void MainWindow::on_actionNew_store_document_triggered()
{
    createWindow<SC_StoreDoc>();
}

void MainWindow::on_actionChange_password_triggered()
{
    DlgChangePass *d = new DlgChangePass(this);
    d->exec();
    delete d;
}

void MainWindow::on_actionInput_documents_triggered()
{
    createWindow<SC_StoreInputDocs>();
}

void MainWindow::on_actionStore_facts_triggered()
{
    createWindow<SC_StoreFact>();
}

void MainWindow::on_actionPartners_triggered()
{
    createWindow<SC_Partners>();
}

void MainWindow::on_actionSale_report_triggered()
{
    createWindow<SC_SaleReport>();
}

void MainWindow::on_actionCurrent_store_triggered()
{
    createWindow<SC_CurrentStore>();
}

void MainWindow::on_actionChange_sale_triggered()
{
    DlgChangeSale::changeSale();
}

void MainWindow::on_actionSettings_triggered()
{
    DlgSettings::settings();
}

void MainWindow::on_actionGoods_groups_triggered()
{
    createWindow<SC_GoodsGroup>();
}
