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

    enum CnxDir { J2N, N2J };
    typedef struct Cnx {
        QProcess *proc;
        QStringList args;
    } Cnx;

    QStatusBar *statusbar;
    QProcess *n2j,*j2n;
    QString binpath;

    Cnx initCnx(CnxDir cnxDir);
    QStringList init_j2n();
    void printOutput(QProcess *proc);
    void cnxControl(CnxDir cnxDir, bool checked);
private slots:
    void on_xButton_released();
    void on_sendButton_toggled(bool checked);
    void cnxStdOut();
    void cnxStdErr();
    void cnxErrorHandler(QProcess::ProcessError error);
    void cnxFinished(int ecode,QProcess::ExitStatus estatus);

    void on_receiveButton_toggled(bool checked);

private:
    Ui::Connector *ui;
};

#endif // CONNECTOR_H
