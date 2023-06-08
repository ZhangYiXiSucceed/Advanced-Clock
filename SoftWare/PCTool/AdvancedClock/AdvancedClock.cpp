#include "AdvancedClock.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextCodec>
#include "ui_AdvancedClock.h"

AdvancedClock::AdvancedClock(QWidget *parent):
    QMainWindow(parent),
    MyTabWidget(new QTabWidget),
    MyTimeShow(new TimeShow(this)),
    MyPictureShow(new PictureShow(this)),
    MyOTA(new OTA(this)),
    ui(new Ui::AdvancedClock)
{
    ui->setupUi(this);
    InitUI();
}

void AdvancedClock::InitUI()
{
    QHBoxLayout *Hlayout = new QHBoxLayout;
    Hlayout->addWidget(MyTabWidget);
    QVBoxLayout *Vlayout = new QVBoxLayout;
    Vlayout->addWidget(MyTabWidget);
    ui->centralwidget->setLayout(Vlayout);
    ui->centralwidget->setLayout(Hlayout);

    QFont Ft("Microsoft YaHei");
    Ft.setPointSize(12);

    this->MyTabWidget->setFont(Ft);
    this->MyTabWidget->setStyleSheet("color:rgb(0,0,0)");


    this->MyTabWidget->addTab(MyTimeShow,tr("时钟显示"));
    this->MyTabWidget->addTab(MyPictureShow,tr("图片显示"));
    this->MyTabWidget->addTab(MyOTA,tr("在线升级"));
}

AdvancedClock::~AdvancedClock()
{
    delete ui;
}

