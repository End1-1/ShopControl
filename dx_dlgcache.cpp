#include "dx_dlgcache.h"
#include "ui_dx_dlgcache.h"
#include "sc_cache.h"

DX_DlgCache::DX_DlgCache(const QString &values, const QString &cacheName, QWidget *parent) :
    SC_Dialog(parent),
    ui(new Ui::DX_DlgCache)
{
    ui->setupUi(this);
    QMap<QString, int> &cache = SC_Cache::getCache(cacheName);
    for (QMap<QString, int>::const_iterator it = cache.begin(); it != cache.end(); it++) {
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setText(it.key());
        item->setData(Qt::UserRole, it.value());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        ui->listWidget->addItem(item);
    }
    QStringList l = values.split(",");
    for (QStringList::const_iterator it = l.begin(); it != l.end(); it++) {
        QList<QListWidgetItem*> items = ui->listWidget->findItems(*it, Qt::MatchExactly);
        for (QList<QListWidgetItem*>::iterator il = items.begin(); il != items.end(); il++) {
            (*il)->setCheckState(Qt::Checked);
            ui->checkBox->setChecked(false);
        }
    }
}

DX_DlgCache::~DX_DlgCache()
{
    delete ui;
}

void DX_DlgCache::on_pushButton_2_clicked()
{
    reject();
}

void DX_DlgCache::on_pushButton_clicked()
{
    valueName = "";
    valueId = "";
    bool first = true;
    for (int i = 0; i < ui->listWidget->count(); i++) {
        QListWidgetItem *item = ui->listWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            if (first) {
                first = false;
            } else {
                valueName += ",";
                valueId += ",";
            }
            valueName += item->text();
            valueId += item->data(Qt::UserRole).toString();
        }
    }
    accept();
}

void DX_DlgCache::on_checkBox_clicked(bool checked)
{
    ui->listWidget->disconnect();
    for (int i = 0; i < ui->listWidget->count(); i++)
        ui->listWidget->item(i)->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
    connect(ui->listWidget, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(on_listWidget_itemChanged(QListWidgetItem*)));
    makeListOfCodes();
}

void DX_DlgCache::on_listWidget_itemChanged(QListWidgetItem *item)
{
    Q_UNUSED(item)
    makeListOfCodes();
}

void DX_DlgCache::makeListOfCodes()
{
    ui->lineEdit->clear();
    bool first = true;
    for (int i = 0; i < ui->listWidget->count(); i++) {
        QListWidgetItem *item = ui->listWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            if (first)
                first = false;
            else
                ui->lineEdit->setText(ui->lineEdit->text() + ",");
            ui->lineEdit->setText(ui->lineEdit->text() + item->data(Qt::UserRole).toString());
        }
    }
}
