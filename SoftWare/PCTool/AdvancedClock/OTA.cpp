#include "OTA.h"
#include "ui_OTA.h"

OTA::OTA(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OTA)
{
    ui->setupUi(this);
}

OTA::~OTA()
{
    delete ui;
}
