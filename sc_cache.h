#ifndef SC_CACHE_H
#define SC_CACHE_H

#include <QObject>
#include <QComboBox>
#include <QLineEdit>

class QPComboBox: public QComboBox {
private:
    Q_OBJECT
    Q_PROPERTY (QString cacheName READ getCacheName WRITE setCacheName)
    Q_PROPERTY (QString sqlQuery READ getSqlQuery WRITE setSqlQuery)
    QString m_cacheName;
    QString m_sqlQuery;
    QString getCacheName();
    void setCacheName(const QString &cacheName, const QString &sqlQuery = "");
    QString getSqlQuery();
    void setSqlQuery(const QString &sqlQuery);
public:
    QPComboBox(QWidget *parent = 0);
    int currentId();
    void setCurrentId(int id);
};

class QPLineEdit: public QLineEdit {
public:
    QPLineEdit(QWidget *parent = 0);
    int toInt();
    void setInt(int value);
};

class SC_Cache
{
private:
    static void queryCache(const QString &cacheName, const QString &sqlQuery);
public:
    SC_Cache();
    static QMap<QString, QMap<QString, int> > m_cache;
    static QMap<QString, bool> m_cacheLoadStatus;
    static QMap<QString, int> &getCache(const QString &cacheName, const QString &sqlQuery = "");
};

#endif // SC_CACHE_H
