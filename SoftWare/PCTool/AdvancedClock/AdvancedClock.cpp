#include "AdvancedClock.h"
#include "ui_AdvancedClock.h"

AdvancedClock::AdvancedClock(QWidget *parent):
    QMainWindow(parent),
    MyTabWidget(new QTabWidget),
    ui(new Ui::AdvancedClock)
{
    ui->setupUi(this);
}

AdvancedClock::~AdvancedClock()
{
    delete ui;
}

