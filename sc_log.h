#ifndef SC_LOG_H
#define SC_LOG_H

#include <QMainWindow>

namespace Ui {
class SC_Log;
}

class SC_Log : public QMainWindow
{
    Q_OBJECT

public:
    explicit SC_Log(QWidget *parent = 0);
    ~SC_Log();
    virtual QString title();
private:
    Ui::SC_Log *ui;
};

#endif // SC_LOG_H
