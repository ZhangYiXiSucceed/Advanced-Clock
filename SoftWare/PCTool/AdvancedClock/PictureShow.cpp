#include "PictureShow.h"
#include "ui_PictureShow.h"

PictureShow::PictureShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureShow)
{
    ui->setupUi(this);
}

PictureShow::~PictureShow()
{
    delete ui;
}
