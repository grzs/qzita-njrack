#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QWidget>
#include <QProcess>

namespace Ui {
class Connector;
}

class Connector : public QWidget
{
    Q_OBJECT

public:
    explicit Connector(QWidget *parent = nullptr);
    ~Connector();    

    QProcess *cnx;

    void initCnx();

private slots:
    void on_pushButton_released();
    void on_sendButton_toggled(bool checked);
    void cnxOutputHandler();

private:
    Ui::Connector *ui;
};

#endif // CONNECTOR_H
