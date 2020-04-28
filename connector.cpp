#include "connector.h"
#include "ui_connector.h"
#include <QDebug>
#include <QFileInfo>

Connector::Connector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Connector)
{
    ui->setupUi(this);

    QString binpath = QCoreApplication::applicationDirPath();
    n2j_program = binpath+"/zita-n2j";
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
    QString message = ui->name->text();
    QString cmd = QFileInfo(n2j_program).fileName();
    if (checked) {
        QStringList args = initCnx();
        n2j->start(n2j_program, args);

        cmd.append(" "+args.join(" "));
        message.append(": ...starting "+ cmd);
    } else {
        cmd.append(" "+n2j->arguments().join(" "));
        n2j->kill();
        message.append(": ...closing "+ cmd);
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
