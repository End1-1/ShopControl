#ifndef SC_LOGIN_H
#define SC_LOGIN_H

#include <QDialog>

namespace Ui {
class SC_Login;
}

class SC_Login : public QDialog
{
    Q_OBJECT

public:
    explicit SC_Login(QWidget *parent = 0);
    ~SC_Login();

private slots:
    void on_btnCancel_clicked();

    void on_btnOK_clicked();

private:
    Ui::SC_Login *ui;
};

#endif // SC_LOGIN_H
