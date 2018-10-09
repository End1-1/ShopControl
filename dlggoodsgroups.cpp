#include "dlggoodsgroups.h"
#include "ui_dlggoodsgroups.h"
#include "common.h"

DlgGoodsGroups::DlgGoodsGroups(QWidget *parent) :
    SC_Dialog(parent),
    ui(new Ui::DlgGoodsGroups)
{
    ui->setupUi(this);
}

DlgGoodsGroups::~DlgGoodsGroups()
{
    delete ui;
}

void DlgGoodsGroups::group(int id)
{
    DlgGoodsGroups *d = new DlgGoodsGroups(fMainWindow);
    if (id > 0) {
        d->load(id);
    }
    d->exec();
    delete d;
}

void DlgGoodsGroups::on_btnCancel_clicked()
{
    reject();
}

void DlgGoodsGroups::on_btnOk_clicked()
{
    if (ui->leCode->text().isEmpty()) {
        ui->leCode->setText(QString::number(db.genId("GEN_GROUPS_ID")));
        db.prepare("insert into groups (id) values (:id)");
        db.bindValue(":id", ui->leCode->text().toInt());
        db.execSQL();
    }
    db.prepare("update groups set fname=:fname, adgcode=:adgcode where id=:id");
    db.bindValue(":fname", ui->leName->text());
    db.bindValue(":adgcode", ui->leADGT->text());
    db.bindValue(":id", ui->leCode->text().toInt());
    db.execSQL();
    db.closeDB();
    QMessageBox::information(this, tr("Information"), tr("Saved"));
}

void DlgGoodsGroups::load(int id)
{
    db.prepare("select fname, adgcode from groups where id=:id");
    db.bindValue(":id", id);
    db.execSQL();
    db.next();
    ui->leCode->setText(QString::number(id));
    ui->leName->setText(db.v_str(0));
    ui->leADGT->setText(db.v_str(1));
    db.closeDB();
}
