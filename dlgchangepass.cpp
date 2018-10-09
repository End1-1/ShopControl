#include "dlgchangepass.h"
#include "ui_dlgchangepass.h"
#include "dbdriver.h"
#include <QMessageBox>

DlgChangePass::DlgChangePass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgChangePass)
{
    ui->setupUi(this);
}

DlgChangePass::~DlgChangePass()
{
    delete ui;
}

void DlgChangePass::on_btnCancel_clicked()
{
    reject();
}

void DlgChangePass::on_btnOk_clicked()
{
    DbDriver db;
    db.openDB();
    db.prepare("select id from users where lower(name)=lower(:username) and password=:password");
    db.bindValue(":username", ui->leUsername->text());
    db.bindValue(":password", ui->leOldPassword->text());
    db.execSQL();
    if (!db.next()) {
        QMessageBox::critical(this, tr("Error"), tr("Access denied"));
        return;
    }
    db.prepare("update users set password=:password, name=lower(:newusername) where lower(name)=lower(:username)");
    db.bindValue(":username", ui->leUsername->text());
    db.bindValue(":newusername", ui->leNewUsername->text());
    db.bindValue(":password", ui->leNewPassword->text());
    db.execSQL();
    db.closeDB();
    QMessageBox::information(this, tr("Info"), tr("Success!"));
    accept();
}
