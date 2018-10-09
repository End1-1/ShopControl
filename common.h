#ifndef COMMON_H
#define COMMON_H

#include "mainwindow.h"
#include <QSettings>

#define float_str(value, f) QString::number(value, 'f', f).remove(QRegExp("\\.0+$")).remove(QRegExp("\\.$"))
#define setx(x, y) QSettings("Shop", "Shop").setValue(x, y)
#define getx(x) QSettings("Shop", "Shop").value(x)

extern MainWindow *fMainWindow;

#endif // COMMON_H
