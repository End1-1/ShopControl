#include "sc_log.h"
#include "ui_sc_log.h"
#include "dbdriver.h"

SC_Log::SC_Log(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SC_Log)
{
    ui->setupUi(this);
    for (QStringList::const_iterator it = m_lastQuery.begin(); it != m_lastQuery.end(); it++) {
        QTextCursor c = QTextCursor(ui->txt->document());
        c.movePosition(QTextCursor::End);
        c.insertText(*it + "\r\n");
    }
}

SC_Log::~SC_Log()
{
    delete ui;
}

QString SC_Log::title()
{
    return tr("Log");
}
