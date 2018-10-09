#include "sc_login.h"
#include "ui_sc_login.h"
#include "dbdriver.h"
#include <QMessageBox>

SC_Login::SC_Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SC_Login)
{
    ui->setupUi(this);
}

SC_Login::~SC_Login()
{
    delete ui;
}

void SC_Login::on_btnCancel_clicked()
{
    reject();
}

void SC_Login::on_btnOK_clicked()
{
    DbDriver db;
    db.openDB();
    db.execSQL("select id from users where lower(name)=lower('" + ui->leLogin->text() + "') and password='" + ui->lePwd->text() + "'");
    if (db.next()) {
        accept();
    } else {
        QMessageBox::critical(this, tr("Login error"), tr("Login failed"));
    }
}
