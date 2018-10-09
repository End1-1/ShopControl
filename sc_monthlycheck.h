#ifndef SC_MONTHLYCHECK_H
#define SC_MONTHLYCHECK_H

#include "sc_grid.h"

#define QTY_TILL 0
#define QTY_IN 1
#define QTY_OUT 2
#define QTY_STORE 3
#define QTY_FACT 4
#define QTY_DIFF 5

class Goods : public QObject {
    Q_OBJECT
public:
    int row;
    int code;
    QString type;
    QString name;
    double qty[6];
    void setQty(int field, double q);
signals:
    void qtyChanged();
};

class SC_MonthlyCheck : public SC_Grid
{
    Q_OBJECT
private:
    int m_docId;
    QMap<int, Goods*> m_goods;
    void addGoods(Goods *g);
    void getQty(int docType, int field, QDate d1, QDate d2);
    int getIdByBarcode();
public:
    SC_MonthlyCheck(QWidget *parent = 0);
    virtual void aPrepareSql();
    virtual void aNew();
    virtual void aEdit();
    virtual QString title();
public slots:
    void updateRow();
};

#endif // SC_MONTHLYCHECK_H
