#include "connector.h"
#include "ui_connector.h"

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
