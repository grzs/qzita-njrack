#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QWidget>
#include <QProcess>
#include <QStatusBar>

namespace Ui {
class Connector;
}

class Connector : public QWidget
{
    Q_OBJECT

public:
    explicit Connector(QWidget *parent = nullptr);
    ~Connector();    

    QStatusBar *statusbar;
    QProcess *n2j,*j2n;
    QString binpath;

    QStringList initCnx();
    QStringList init_j2n();
    void printOutput();
private slots:
    void on_pushButton_released();
    void on_sendButton_toggled(bool checked);
    void cnxStdOut();
    void cnxStdErr();
    void cnxErrorHandler(QProcess::ProcessError error);
    void cnxFinished(int ecode,QProcess::ExitStatus estatus);

private:
    Ui::Connector *ui;
};

#endif // CONNECTOR_H
