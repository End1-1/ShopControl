#include "sc_storeinputdocs.h"
#include "common.h"
#include "sc_storedoc.h"

SC_StoreInputDocs::SC_StoreInputDocs(QWidget *parent) :
    SC_Grid(parent)
{
    setWindowTitle(tr("Documents in the store"));
    setFilter("date1", new DX_TableFilter(DX_TableFilter::wtDateEdit, tr("Date"), true, this));
    setFilter("date2", new DX_TableFilter(DX_TableFilter::wtDateEdit, tr("Date"), false, this));
    setFilter("partner", new DX_TableFilter(DX_TableFilter::wtLineEdit, tr("Group"), true, this));
    f("date1")->setValue(QDate::currentDate());
    f("date2")->setValue(QDate::currentDate());
    t->setColumn("ID", tr("Number"), 80);
    t->setColumn("FDATE", tr("Date"), 100);
    t->setColumn("LLC", tr("Partner"), 300);
    t->setColumn("FAMOUNT", tr("Amount"), 150, true);
    aPrepareSql();
    openQuery();
}

void SC_StoreInputDocs::aPrepareSql()
{
    QString sql = QString("select d.id, d.fdate, p.llc, d.famount "
            "from sth d "
            "left join partners p on d.fpartner=p.id "
            "where d.fdate between '%1' and '%2' and d.ftype=2")
            .arg(f("date1")->getDate().toString("dd.MM.yyyy"))
            .arg(f("date2")->getDate().toString("dd.MM.yyyy"));
    if (f("partner")->getValue().toInt() > 0) {
        sql += " and d.fpartner=" + f("partner")->getValue().toString();
    }
    sql += fAnd;
    m_sqlQuery = sql;
}

void SC_StoreInputDocs::aNew()
{
    fMainWindow->createWindow<SC_StoreDoc>();
}

void SC_StoreInputDocs::aEdit()
{
    int id = getId().toInt();
    if (!id) {
        return;
    }
    SC_StoreDoc *sd = fMainWindow->createWindow<SC_StoreDoc>();
    sd->load(id);
}

bool SC_StoreInputDocs::aDel()
{
    int id = getId().toInt();
    if (id > 0) {
        if (QMessageBox::warning(this, tr("Warning"), tr("Confirm deletion"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
            return true;
        db.prepare("delete from stb where fdoc=:fdoc");
        db.bindValue(":fdoc", id);
        db.execSQL();
        db.prepare("delete from sth where id=:id");
        db.bindValue(":id", id);
        db.execSQL();
        db.closeDB();
    }
    return true;
}

void SC_StoreInputDocs::setFirstDate(const QDate &date)
{
    f("date1")->setValue(date);
    aPrepareSql();
    openQuery();
}
