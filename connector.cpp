#include "connector.h"
#include "ui_connector.h"
#include <QDebug>

Connector::Connector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Connector)
{
    ui->setupUi(this);

}

Connector::~Connector()
{
    delete ui;
}

void Connector::initCnx()
{
    QString program = "echo";
    QStringList args = {"-n", ui->name->text()};
    //QString program = "ping";
    //QStringList args = {"-i","1.5","google.com"};
    //args << WD;

    cnx = new QProcess(this);
    QObject::connect(cnx, SIGNAL(readyRead()),
                     this, SLOT(cnxOutputHandler()));

    cnx->start(program, args);
}

void Connector::on_pushButton_released()
{
    this->~Connector();
}

void Connector::on_sendButton_toggled(bool checked)
{
    if (checked) {
        initCnx();
        qDebug() << "on";
    } else {
        cnx->kill();
        qDebug() << "off";
    }
}

void Connector::cnxOutputHandler()
{
    qDebug() << cnx->readAllStandardOutput();
}
