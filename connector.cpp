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
//    if (n2j->state() == QProcess::Running) {
//        n2j->kill();
//    }
    delete ui;
}

QStringList Connector::initCnx()
{
    n2j = new QProcess(this);

    QObject::connect(n2j, SIGNAL(readyReadStandardOutput()),
                     this, SLOT(cnxStdOut()));
    QObject::connect(n2j, SIGNAL(readyReadStandardError()),
                     this, SLOT(cnxStdErr()));
    QObject::connect(n2j, SIGNAL(errorOccurred(QProcess::ProcessError)),
                     this, SLOT(cnxErrorHandler(QProcess::ProcessError)));
    QObject::connect(n2j, SIGNAL(finished(int,QProcess::ExitStatus)),
                     this, SLOT(cnxFinished(int,QProcess::ExitStatus)));

    QStringList args;

    // adding arguments
    args << "--jname" << "n2j-"+ui->name->text();
    args << ui->listen_ip->text() << ui->port->text();

    return args;
}

void Connector::on_pushButton_released()
{
    this->~Connector();
}

void Connector::on_sendButton_toggled(bool checked)
{
    // if ...
    QProcess *cnx;
    QString bin = "zita-n2j";
    // endif

    QStringList cmd = {bin};
    QString message = ui->name->text() + ": ";

    if (checked) {
        QStringList args = initCnx();
        cnx = n2j; // TODO
        cmd.append(args);
        message.append("...starting "+ cmd.join(" "));

        QString fullpath = QDir(binpath).filePath(bin);
        cnx->start(fullpath, args);
    } else {
        cnx = n2j; // TODO
        cmd.append(cnx->arguments());
        message.append("...closing "+ cmd.join(" "));

        cnx->kill();
    }
    qDebug() << message;
    statusbar->showMessage(message,3000);
}

void Connector::printOutput()
{
    //qDebug() << n2j->readChannel();
    do {
        QString message = ui->name->text() + ": ";
        message.append(n2j->readLine());
        qDebug() << message;
        statusbar->showMessage(message);
    } while (!n2j->atEnd());
}

void Connector::cnxStdOut()
{
    printOutput();
}

void Connector::cnxStdErr()
{
    n2j->setReadChannel(QProcess::StandardError);
    printOutput();
    n2j->setReadChannel(QProcess::StandardOutput);
}

void Connector::cnxErrorHandler(QProcess::ProcessError error)
{
    qDebug() << error << n2j->readAllStandardError();
}

void Connector::cnxFinished(int ecode, QProcess::ExitStatus estatus)
{
    if (ui->sendButton->isChecked()) {
        ui->sendButton->toggle();
    }
    qDebug() << estatus << ecode;
}
