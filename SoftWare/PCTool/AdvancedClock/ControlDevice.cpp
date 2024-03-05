#include "ControlDevice.h"
#include "ui_ControlDevice.h"

ControlDevice::ControlDevice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlDevice)
{
    ui->setupUi(this);
}

ControlDevice::~ControlDevice()
{
    delete ui;
}
