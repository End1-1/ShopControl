#include "dlgsettings.h"
#include "ui_dlgsettings.h"
#include "common.h"
#include <QMessageBox>

DlgSettings::DlgSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSettings)
{
    ui->setupUi(this);
    ui->leDB->setText(getx("dbpath").toString());
    ui->leTaxIP->setText(getx("taxip").toString());
    ui->leTaxPort->setText(getx("taxport").toString());
    ui->leTaxPass->setText(getx("taxpass").toString());
}

DlgSettings::~DlgSettings()
{
    delete ui;
}

void DlgSettings::settings()
{
    DlgSettings *d = new DlgSettings(fMainWindow);
    d->exec();
    delete d;
}

void DlgSettings::on_btnCancel_clicked()
{
    reject();
}

void DlgSettings::on_btnOK_clicked()
{
    setx("dbpath", ui->leDB->text());
    setx("taxip", ui->leTaxIP->text());
    setx("taxport", ui->leTaxPort->text());
    setx("taxpass", ui->leTaxPass->text());
    QMessageBox::information(this, tr("Information"), tr("Saved"));
}
