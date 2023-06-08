#include "TimeShow.h"
#include "ui_TimeShow.h"

TimeShow::TimeShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeShow)
{
    ui->setupUi(this);
}

TimeShow::~TimeShow()
{
    delete ui;
}
