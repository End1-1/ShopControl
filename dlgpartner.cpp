#include "dlgpartner.h"
#include "ui_dlgpartner.h"
#include "common.h"

DlgPartner::DlgPartner(QWidget *parent) :
    SC_Dialog(parent),
    ui(new Ui::DlgPartner)
{
    ui->setupUi(this);
}

DlgPartner::~DlgPartner()
{
    delete ui;
}

void DlgPartner::edit(int id)
{
    DlgPartner *d = new DlgPartner(fMainWindow);
    if (id > 0) {
        d->db.prepare("select hvhh, llc, additional from partners where id=:id");
        d->db.bindValue(":id", id);
        d->db.execSQL();
        d->db.next();
        d->ui->leCode->setText(QString::number(id));
        d->ui->leTaxCode->setText(d->db.v_str(0));
        d->ui->leName->setText(d->db.v_str(1));
        d->ui->leAdditional->setText(d->db.v_str(2));
    }
    d->exec();
    delete d;
}

void DlgPartner::on_btnCancel_clicked()
{
    reject();
}

void DlgPartner::on_btnOk_clicked()
{
    if (ui->leCode->text().isEmpty()) {
        ui->leCode->setText(QString::number(db.genId("GEN_PARTNERS_ID")));
        db.prepare("insert into partners (id) values (:id)");
        db.bindValue(":id", ui->leCode->text().toInt());
        db.execSQL();
    }
    db.prepare("update partners set hvhh=:hvhh, llc=:llc, additional=:additional where id=:id");
    db.bindValue(":hvhh", ui->leTaxCode->text());
    db.bindValue(":llc", ui->leName->text());
    db.bindValue(":additional", ui->leAdditional->text());
    db.bindValue(":id", ui->leCode->text().toInt());
    db.execSQL();
    db.closeDB();
    accept();
}
