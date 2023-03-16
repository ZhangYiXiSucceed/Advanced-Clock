#include "AdvancedClock.h"
#include "ui_AdvancedClock.h"

AdvancedClock::AdvancedClock(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdvancedClock)
{
    ui->setupUi(this);
}

AdvancedClock::~AdvancedClock()
{
    delete ui;
}

