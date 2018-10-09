#-------------------------------------------------
#
# Project created by QtCreator 2015-12-18T12:49:16
#
#-------------------------------------------------

QT       += core gui sql printsupport xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShopControl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    sc_grid.cpp \
    sc_goods.cpp \
    ../Shop/dbdriver.cpp \
    sc_tablewidget.cpp \
    sc_dialog.cpp \
    sc_editgoods.cpp \
    sc_cache.cpp \
    sc_docs.cpp \
    dx_tablefilter.cpp \
    sc_dlgfilter.cpp \
    dx_dlgcache.cpp \
    sc_login.cpp \
    sc_log.cpp \
    sc_monthlycheck.cpp \
    sc_storedoc.cpp \
    sc_goodsqtyamount.cpp \
    dlgchangepass.cpp \
    sc_storeinputdocs.cpp \
    common.cpp \
    sc_storefact.cpp \
    sc_partners.cpp \
    dlgpartner.cpp \
    sc_salereport.cpp \
    sc_storebyperiod.cpp \
    sc_currentstore.cpp \
    dlgchangesale.cpp \
    sc_toolbutton.cpp \
    dlgsettings.cpp \
    sc_goodsgroup.cpp \
    dlggoodsgroups.cpp

HEADERS  += mainwindow.h \
    sc_grid.h \
    dbdriver.h \
    sc_goods.h \
    ../Shop/dbdriver.h \
    sc_tablewidget.h \
    sc_dialog.h \
    sc_editgoods.h \
    sc_cache.h \
    sc_docs.h \
    dx_tablefilter.h \
    sc_dlgfilter.h \
    dx_dlgcache.h \
    sc_login.h \
    sc_log.h \
    sc_monthlycheck.h \
    sc_storedoc.h \
    sc_goodsqtyamount.h \
    dlgchangepass.h \
    sc_storeinputdocs.h \
    common.h \
    sc_storefact.h \
    sc_partners.h \
    dlgpartner.h \
    sc_salereport.h \
    sc_storebyperiod.h \
    sc_currentstore.h \
    dlgchangesale.h \
    sc_toolbutton.h \
    dlgsettings.h \
    sc_goodsgroup.h \
    dlggoodsgroups.h

FORMS    += mainwindow.ui \
    sc_grid.ui \
    sc_editgoods.ui \
    dx_dlgcache.ui \
    sc_login.ui \
    sc_log.ui \
    sc_storedoc.ui \
    sc_goodsqtyamount.ui \
    dlgchangepass.ui \
    dlgpartner.ui \
    dlgchangesale.ui \
    dlgsettings.ui \
    dlggoodsgroups.ui

RESOURCES += \
    res.qrc

ICON = app.ico

RC_FILE = rc.rc
