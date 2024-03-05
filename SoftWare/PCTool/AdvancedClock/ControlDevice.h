#ifndef CONTROLDEVICE_H
#define CONTROLDEVICE_H

#include <QWidget>

namespace Ui {
class ControlDevice;
}

class ControlDevice : public QWidget
{
    Q_OBJECT

public:
    explicit ControlDevice(QWidget *parent = nullptr);
    ~ControlDevice();

private:

    Ui::ControlDevice *ui;
};

#endif // CONTROLDEVICE_H
