#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    add_connWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_connWidget()
{
    Connector* conn = new Connector(this);
    ui->connLayout->insertWidget(-1,conn);
}

void MainWindow::on_addButton_pressed()
{
    add_connWidget();
}
