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
    InitConnect();
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

    statusBar()->setFont(Ft);

    InternetIP=new QLabel();
    InternetIP->setText(tr("IP：无"));
    this->ui->statusbar->addPermanentWidget(InternetIP);
    Port=new QLabel();
    Port->setText(tr("端口：无"));
    this->ui->statusbar->addPermanentWidget(Port);

    TextMessage+=tr("提示消息：");
    TextMessage+=tr("界面创建成功!");
    statusBar()->showMessage(TextMessage,1500);
}

void AdvancedClock::InitConnect()
{
    connect(MyTimeShow,SIGNAL(ShowParameter(QString,QString)),this,SLOT(ShowParameter(QString,QString)));
    connect(MyTimeShow,SIGNAL(ShowSystemMessage(QString,uint16_t)),this,SLOT(ShowSystemMessage(QString,uint16_t)));

    connect(MyOTA,SIGNAL(SendReq2Device(QByteArray)),MyTimeShow,SLOT(SendData2Device(QByteArray)));
    connect(MyTimeShow,SIGNAL(SendData2OTA(QByteArray)),MyOTA,SLOT(RspDataProcess(QByteArray)));
}

/************************************************************************************************************
函数名：ShowSystemMessage(QString Message,uint16_t ShowTime)
参数：QString Message,uint16_t ShowTime，消息和显示消息的时间，单位ms
函数功能：状态栏左侧显示消息
返回值：无
************************************************************************************************************/
void AdvancedClock::ShowSystemMessage(QString Message,uint16_t ShowTime)
{
    statusBar()->showMessage(Message,ShowTime);
}

/************************************************************************************************************
函数名：ShowParameter(QString TempIP,QString TempPort)
参数：QString TempIP,QString TempPort，IP和端口
函数功能：状态栏右侧永久显示波特率和端口号。
返回值：无
************************************************************************************************************/
void AdvancedClock::ShowParameter(QString TempIP, QString TempPort)
{
   InternetIP->setText(tr("ServerIP：")+TempIP);
   this->ui->statusbar->addPermanentWidget(InternetIP);
   Port->setText(tr("端口：")+TempPort);
   this->ui->statusbar->addPermanentWidget(Port);
}
AdvancedClock::~AdvancedClock()
{
    delete ui;
}

