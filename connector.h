#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QWidget>

namespace Ui {
class Connector;
}

class Connector : public QWidget
{
    Q_OBJECT

public:
    explicit Connector(QWidget *parent = nullptr);
    ~Connector();

private:
    Ui::Connector *ui;
};

#endif // CONNECTOR_H
