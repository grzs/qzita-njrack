#include "connector.h"
#include "ui_connector.h"
#include <QDebug>
#include <QDir>

Connector::Connector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Connector)
{
    ui->setupUi(this);

    binpath = QCoreApplication::applicationDirPath();
}

Connector::~Connector()
{
    delete ui;
}

Connector::Cnx Connector::initCnx(CnxDir cnxDir)
{
    Cnx cnx;
    if (! ui->name->text().isEmpty()) {
        cnx.args << "--jname" << ui->name->text();
    }

    switch (cnxDir) {
    case CnxDir::J2N:
        j2n = new QProcess(this);
        cnx.proc = j2n;
        cnx.args << ui->remote_ip->text() << ui->port->text();
        break;
    case CnxDir::N2J:
        n2j = new QProcess(this);
        cnx.proc = n2j;
        cnx.args << ui->listen_ip->text() << ui->port->text();
        break;
    }


    QObject::connect(cnx.proc, SIGNAL(readyReadStandardOutput()),
                     this, SLOT(cnxStdOut()));
    QObject::connect(cnx.proc, SIGNAL(readyReadStandardError()),
                     this, SLOT(cnxStdErr()));
    QObject::connect(cnx.proc, SIGNAL(errorOccurred(QProcess::ProcessError)),
                     this, SLOT(cnxErrorHandler(QProcess::ProcessError)));
    QObject::connect(cnx.proc, SIGNAL(finished(int,QProcess::ExitStatus)),
                     this, SLOT(cnxFinished(int,QProcess::ExitStatus)));

    return cnx;
}

void Connector::cnxControl(CnxDir cnxDir, bool checked)
{
    Cnx cnx; // for process init
    QProcess *proc; // to know what to kill
    QString bin;

    switch (cnxDir) {
    case CnxDir::J2N:
        bin = "zita-j2n";
        proc = j2n;
        break;
    case CnxDir::N2J:
        proc = n2j;
        bin = "zita-n2j";
        break;
    }

    QStringList cmd = {bin};
    QString message = ui->name->text() + ": ";

    if (checked) {
        cnx = initCnx(cnxDir);
        cmd.append(cnx.args);
        message.append("...starting "+ cmd.join(" "));

        QString fullpath = QDir(binpath).filePath(bin);
        cnx.proc->start(fullpath, cnx.args);
    } else {
        cmd.append(proc->arguments());
        message.append("...closing "+ cmd.join(" "));

        proc->kill();
    }
    qDebug() << message;
    statusbar->showMessage(message,3000);
}

void Connector::printOutput()
{
    //qDebug() << j2n->readChannel();
    do {
        QString message = ui->name->text() + ": ";
        message.append(j2n->readLine());
        qDebug() << message;
        statusbar->showMessage(message);
    } while (!j2n->atEnd());
}

void Connector::cnxStdOut()
{
    printOutput();
}

void Connector::cnxStdErr()
{
    j2n->setReadChannel(QProcess::StandardError);
    printOutput();
    j2n->setReadChannel(QProcess::StandardOutput);
}

void Connector::cnxErrorHandler(QProcess::ProcessError error)
{
    qDebug() << error << j2n->readAllStandardError();
}

void Connector::cnxFinished(int ecode, QProcess::ExitStatus estatus)
{
    QProcess *proc = qobject_cast<QProcess *>(QObject::sender());
    QPushButton *button = nullptr;
    QString dir = proc->program().right(3);
    if (dir == "j2n") {
        button = ui->sendButton;
    } else if (dir == "n2j") {
        button = ui->receiveButton;
    }

    if (button->isChecked()) {
        button->toggle();
    }
    qDebug() << estatus << ecode;
}

void Connector::on_xButton_released()
{
    this->~Connector();
}

void Connector::on_sendButton_toggled(bool checked)
{
    cnxControl(CnxDir::J2N,checked);
}

void Connector::on_receiveButton_toggled(bool checked)
{
    cnxControl(CnxDir::N2J,checked);
}
