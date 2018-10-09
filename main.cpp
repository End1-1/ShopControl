#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFontDatabase fontDb;
    int fontId = fontDb.addApplicationFont(a.applicationDirPath() + "/ahuni.ttf");
    QString fontFamily = fontDb.applicationFontFamilies(fontId).at(0);
    QFont font = fontDb.font(fontFamily, "Normal", 11);
    a.setFont(font);
    a.setWindowIcon(QIcon(":/app.ico"));
    QTranslator t;
    t.load(":/ShopControl.qm");
    a.installTranslator(&t);
    MainWindow w;
    w.show();
    return a.exec();
}
