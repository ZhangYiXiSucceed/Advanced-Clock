#include "AdvancedClock.h"
#include "ui_AdvancedClock.h"

AdvancedClock::AdvancedClock(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdvancedClock),
    MyTabWidget(new QTabWidget)
{
    ui->setupUi(this);
}

AdvancedClock::~AdvancedClock()
{
    delete ui;
}

