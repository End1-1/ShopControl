#ifndef DX_DLGCACHE_H
#define DX_DLGCACHE_H

#include "sc_dialog.h"
#include <QListWidgetItem>

namespace Ui {
class DX_DlgCache;
}

class DX_DlgCache : public SC_Dialog
{
    Q_OBJECT

public:
    explicit DX_DlgCache(const QString &values, const QString &cacheName, QWidget *parent = 0);
    ~DX_DlgCache();
    QString valueName;
    QString valueId;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_checkBox_clicked(bool checked);

    void on_listWidget_itemChanged(QListWidgetItem *item);

private:
    Ui::DX_DlgCache *ui;
    void makeListOfCodes();
};

#endif // DX_DLGCACHE_H
