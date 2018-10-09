#include "sc_cache.h"
#include "dbdriver.h"

QMap<QString, QMap<QString, int> > SC_Cache::m_cache;
QMap<QString, bool> SC_Cache::m_cacheLoadStatus;

void SC_Cache::queryCache(const QString &cacheName, const QString &sqlQuery)
{
    m_cache[cacheName] = QMap<QString, int>();
    m_cacheLoadStatus[cacheName] = false;
    DbDriver *d = new DbDriver();
    if (!d->openDB()) {
        delete d;
        return;
    }
    if (!d->execSQL(sqlQuery)) {
        delete d;
        return;
    }
    while (d->next())
        m_cache[cacheName][d->v_str(1)] = d->v_int(0);
    d->closeDB();
    delete d;
    m_cacheLoadStatus[cacheName] = true;
}

SC_Cache::SC_Cache()
{

}

QMap<QString, int> &SC_Cache::getCache(const QString &cacheName, const QString &sqlQuery)
{
    QString sql = sqlQuery.length() ? sqlQuery : "select id, fname from " + cacheName;
    if (!m_cache.contains(cacheName))
        queryCache(cacheName, sql);
    else if (m_cacheLoadStatus[cacheName])
        queryCache(cacheName, sql);
    return m_cache[cacheName];
}


QString QPComboBox::getCacheName()
{
    return m_cacheName;
}

void QPComboBox::setCacheName(const QString &cacheName, const QString &sqlQuery)
{
    m_cacheName = cacheName;
    QMap<QString, int> &m = SC_Cache::getCache(cacheName, sqlQuery);
    for (QMap<QString, int>::const_iterator it = m.begin(); it != m.end(); it++)
        addItem(it.key(), it.value());
    setCurrentIndex(-1);
}

QString QPComboBox::getSqlQuery()
{
    return m_sqlQuery;
}

void QPComboBox::setSqlQuery(const QString &sqlQuery)
{
    m_sqlQuery = sqlQuery;
}

QPComboBox::QPComboBox(QWidget *parent) :
    QComboBox(parent)
{

}

int QPComboBox::currentId()
{
    return itemData(currentIndex()).toInt();
}

void QPComboBox::setCurrentId(int id)
{
    int index = findData(id);
    setCurrentIndex(index);
}

QPLineEdit::QPLineEdit(QWidget *parent) :
    QLineEdit(parent)
{

}

int QPLineEdit::toInt()
{
    return text().toInt();
}

void QPLineEdit::setInt(int value)
{
    setText(QString::number(value));
}
