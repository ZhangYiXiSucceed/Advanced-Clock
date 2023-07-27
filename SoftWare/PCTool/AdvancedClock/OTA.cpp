#include "OTA.h"
#include "ui_OTA.h"
#include <QMessageBox>
#include <iostream>
#include <iomanip>
#include <QFileDialog>
#include <QFile>
#include <QDateTime>
using namespace std;

OTA::OTA(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OTA)
{
    ui->setupUi(this);

    MyStartConnectTimer = new QTimer;
    MyStartConnectTimer->start(5000);
    MyStartConnectTimer->setSingleShot(true);

    ota_info_manager.BinBuf = new quint8[64*1024*1024];
    ota_info_manager.state = START_OTA_TRNASMIT_INFO;

    MyThread = new QThreadRun;
    MyThread->SetSwitch(true);
    MyThread->start();

    set_ota_transmit_state(OTA_TRANSMIT_END_RSP);

    InitUI();
    InitConnect();
    PrintMsg();

    ui->UpgradProgressBar->setRange(0,100);
}

void OTA::InitUI()
{
    QFont Ft("Microsoft YaHei");
    Ft.setPointSize(12);

    ui->OpenDevice->setEnabled(true);
    ui->CloseDevice->setEnabled(false);

    ui->SoftwareVersion->setFont(Ft);
    ui->HardwareVersion->setFont(Ft);

    ui->SoftwareLabel->setFont(Ft);
    ui->HardwareLabel->setFont(Ft);
    ui->BarLable->setFont(Ft);
    ui->UpgradBinFileNameLable->setFont(Ft);
    ui->UpgradBinFileSizeLable->setFont(Ft);
    ui->FileLable->setFont(Ft);
    ui->UpgradBinFileDateTimeLable->setFont(Ft);

    ui->JumpDevice->setFont(Ft);
    ui->JumpDevice->setIcon(DeviceJumpPic);
    ui->JumpDevice->setIconSize(DeviceJumpPic.size()/4);

    ui->GetVersion->setFont(Ft);
    ui->GetVersion->setIcon(DeviceVerSionPic);
    ui->GetVersion->setIconSize(DeviceVerSionPic.size()/4);

    ui->ResetDevice->setFont(Ft);
    ui->ResetDevice->setIcon(DeviceVerSionPic);
    ui->ResetDevice->setIconSize(DeviceVerSionPic.size()/4);

    ui->ConnectDevice->setFont(Ft);
    ui->ConnectDevice->setIcon(DeviceConnectPic);
    ui->ConnectDevice->setIconSize(DeviceConnectPic.size()/4);

    ui->OpenDevice->setFont(Ft);
    ui->OpenDevice->setIcon(DeviceOpenPic);
    ui->OpenDevice->setIconSize(DeviceOpenPic.size()/4);

    ui->CloseDevice->setFont(Ft);
    ui->CloseDevice->setIcon(DeviceClosePic);
    ui->CloseDevice->setIconSize(DeviceClosePic.size()/4);

    ui->SetConnectMode->setFont(Ft);
    ui->SetConnectMode->setIcon(DeviceSetPic);
    ui->SetConnectMode->setIconSize(DeviceSetPic.size()/4);

    ui->SelectUpgradBinFile->setFont(Ft);
    ui->SelectUpgradBinFile->setIcon(SelectUpgradeFilePic);
    ui->SelectUpgradBinFile->setIconSize(SelectUpgradeFilePic.size()/4);
    ui->SelectUpgradBinFile->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->StartUpgrade->setFont(Ft);
    ui->StartUpgrade->setIcon(StartUpgradePic);
    ui->StartUpgrade->setIconSize(StartUpgradePic.size()/4);
}
void OTA::InitConnect()
{
    connect(ui->ResetDevice,SIGNAL(clicked(bool)),this,SLOT(ResetDeviceCmd()));
    connect(ui->ConnectDevice,SIGNAL(clicked(bool)),this,SLOT(ConnectDeviceCmd()));
    connect(ui->JumpDevice,SIGNAL(clicked(bool)),this,SLOT(JumpDeviceCmd()));
    connect(ui->GetVersion,SIGNAL(clicked(bool)),this,SLOT(GetVersionCmd()));
    connect(ui->SetConnectMode,SIGNAL(clicked(bool)),this, SLOT(SetConnectMode()));

    connect(ui->OpenDevice,SIGNAL(clicked(bool)),this,SLOT(OpenDevice()));
    connect(ui->CloseDevice,SIGNAL(clicked(bool)),this,SLOT(CloseDevice()));

    connect(ui->SelectUpgradBinFile,SIGNAL(clicked(bool)),this,SLOT(SelectOTABin()));
    connect(ui->StartUpgrade,SIGNAL(clicked(bool)),this,SLOT(StartUpgrade()));

    connect(MyStartConnectTimer,SIGNAL(timeout()),this,SLOT(OpenDevice()));

    connect(MyThread,SIGNAL(RunFunc()),this,SLOT(UpgradeBinThread()));
}

void PrintMsg()
{
    static int Count = 0;
    cout << "PrintMsg " << Count++ << endl;
    QThread::msleep(1000);
}

void OTA::set_ota_transmit_state(ota_transmit_state_t state)
{
    ota_info_manager.state = state;
}

ota_transmit_state_t OTA::get_ota_transmit_state()
{
    return ota_info_manager.state;
}
void OTA::UpgradeBinThread()
{
    switch(ota_info_manager.state)
    {
        case START_OTA_TRNASMIT_INFO:
        {
            TransmitBinInfo();
            set_ota_transmit_state(START_OTA_TRNASMIT_INFO_RSP);
            emit ShowSystemMessage(tr(" start ota transmit info"),1500);
            ota_info_manager.curr_package_num = 0;
        }
        break;
        case START_OTA_TRNASMIT_INFO_RSP:
        {

        }
        break;
        case OTA_TRANSMIT_DATA:
        {
            if(ota_info_manager.curr_package_num < ota_info_manager.package_num)
            {
                TransmitBinData(ota_info_manager.curr_package_num);
                ota_info_manager.curr_package_num++;
                set_ota_transmit_state(OTA_TRANSMIT_DATA_RSP);
                ui->UpgradProgressBar->setValue((ota_info_manager.curr_package_num*100)/ota_info_manager.package_num);
            }
            else
            {
                set_ota_transmit_state(OTA_TRANSMIT_END);
            }
        }
        break;
        case OTA_TRANSMIT_DATA_RSP:
        {

        }
        break;
        case OTA_TRANSMIT_END:
        {
            TransmitBinEnd();
            set_ota_transmit_state(OTA_TRANSMIT_END_RSP);
            emit ShowSystemMessage(tr("ota transmit end"),1500);
        }
        break;
        case OTA_TRANSMIT_END_RSP:
        {

        }
        break;
        default:
        {

        }
    }
}

void OTA::OpenDevice()
{
    emit OpenDeviceReq();
    ui->OpenDevice->setEnabled(false);
    ui->CloseDevice->setEnabled(true);
}

void OTA::CloseDevice()
{
    emit CloseDeviceReq();
    ui->OpenDevice->setEnabled(true);
    ui->CloseDevice->setEnabled(false);
}

void OTA::SelectOTABin()
{
    ota_info_manager.FileAddress = QFileDialog::getOpenFileName(this,tr("Open File"),ota_info_manager.FileAddress,tr("(*.bin);;(*.hex);;(*.axf);;(*.txt);;All Files(*.*)"));
    int position = ota_info_manager.FileAddress.toStdString().find_last_of('/');
    ui->UpgradBinFileAddress->setText(ota_info_manager.FileAddress.mid(position+1));
    if(ota_info_manager.FileAddress != NULL)
    {
        QFileInfo *Temp = new QFileInfo(ota_info_manager.FileAddress);
        ota_info_manager.BinSize = Temp->size();
        ui->UpgradBinFileTimeDate->setText(Temp->lastModified().toString("yyyy-MM-dd hh:mm:ss"));
        //添加日期等
        QFile *BinFile = new QFile(ota_info_manager.FileAddress);
        if(BinFile->open(QIODevice::ReadOnly))
        {
            emit ShowSystemMessage(tr("打开文件成功，并获取相关信息！"),1500);
            ui->UpgradBinFileSize->setText(tr("%1 B\n").arg(ota_info_manager.BinSize));
            QByteArray  array = BinFile->readAll();
            memcpy(ota_info_manager.BinBuf, array.data(), ota_info_manager.BinSize);
            BinFile->close();
        }
        delete BinFile;
        delete Temp;
    }
    else
    {
        emit ShowSystemMessage(tr("未打开任何文件！"),1500);
    }
}



void OTA::TransmitBinInfo()
{
    uint8_t buf[sizeof(cmd_msg_frame_t) +  sizeof(ota_package_info_t) + sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + sizeof(ota_package_info_t) + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = START_UPDATE;
    msg->seq = 0;
    msg->data_len = sizeof(ota_package_info_t);

    ota_package_info_t *info =(ota_package_info_t *)(msg+1);
    info->bin_size = ota_info_manager.BinSize;
    info->package_num = (ota_info_manager.BinSize % OTA_ONE_PACKAGE_SIZE)?(ota_info_manager.BinSize/OTA_ONE_PACKAGE_SIZE + 1):(ota_info_manager.BinSize/OTA_ONE_PACKAGE_SIZE);
    info->check_sum = CalCheckSum(ota_info_manager.BinBuf,ota_info_manager.BinSize);

    ota_info_manager.package_num =  info->package_num;

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t) + sizeof(ota_package_info_t));
    uint32_t *check_sum = (uint32_t *)(&buf[sizeof(cmd_msg_frame_t) + sizeof(ota_package_info_t)]);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    emit SendReq2Device(Sendata);
}

void OTA::TransmitBinData(uint16_t cnt)
{
    uint8_t buf[sizeof(cmd_msg_frame_t) +  OTA_ONE_PACKAGE_SIZE + sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + OTA_ONE_PACKAGE_SIZE + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = UPDATE_DATA;
    msg->seq = cnt;
    msg->data_len = OTA_ONE_PACKAGE_SIZE;

    uint8_t *data =(uint8_t *)(msg+1);
    memcpy(data,ota_info_manager.BinBuf+cnt*OTA_ONE_PACKAGE_SIZE,OTA_ONE_PACKAGE_SIZE);

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t) + OTA_ONE_PACKAGE_SIZE);
    uint32_t *check_sum = (uint32_t *)(&buf[sizeof(cmd_msg_frame_t) + OTA_ONE_PACKAGE_SIZE + 1]);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    emit SendReq2Device(Sendata);
}

void OTA::TransmitBinEnd()
{
    uint8_t buf[sizeof(cmd_msg_frame_t) +  sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = UPDATE_END;
    msg->seq = 0x00;
    msg->data_len = 0x00;

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t));
    uint32_t *check_sum = (uint32_t *)(msg+1);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    emit SendReq2Device(Sendata);
}

void OTA::StartUpgrade()
{
    set_ota_transmit_state(START_OTA_TRNASMIT_INFO);
}

void OTA::ResetDeviceCmd()
{
    uint8_t buf[sizeof(cmd_msg_frame_t) +  sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = Reset_Cmd;
    msg->seq = 0x00;
    msg->data_len = 0x00;

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t));
    uint32_t *check_sum = (uint32_t *)(msg+1);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    emit SendReq2Device(Sendata);
}
void OTA::JumpDeviceCmd()
{
    uint8_t buf[sizeof(cmd_msg_frame_t) +  sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = JUMP_CMD;
    msg->seq = 0x00;
    msg->data_len = 0x00;

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t));
    uint32_t *check_sum = (uint32_t *)(msg+1);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    emit SendReq2Device(Sendata);
}


void OTA::ConnectDeviceCmd()
{
    uint8_t buf[sizeof(cmd_msg_frame_t) +  sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = CONNECT_CMD;
    msg->seq = 0x00;
    msg->data_len = 0x00;

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t));
    uint32_t *check_sum = (uint32_t *)(msg+1);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    emit SendReq2Device(Sendata);
}

void OTA::GetVersionCmd()
{
    uint8_t buf[sizeof(cmd_msg_frame_t) +  sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = VERSION_CMD;
    msg->seq = 0x00;
    msg->data_len = 0x00;

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t));
    uint32_t *check_sum = (uint32_t *)(msg+1);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    emit SendReq2Device(Sendata);
}

void OTA::SetConnectMode()
{
    static uint8_t mode = 0;
    uint8_t buf[sizeof(cmd_msg_frame_t) +  sizeof(connect_mode_set_t) + sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + sizeof(connect_mode_set_t) + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = CONNECT_MODE_CMD;
    msg->seq = 0x00;
    msg->data_len = sizeof(connect_mode_set_t);

    connect_mode_set_t *rsp = (connect_mode_set_t *)(msg + 1);
    rsp->mode = mode;

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t) + sizeof(connect_mode_set_t));
    uint32_t *check_sum = (uint32_t *)(rsp+1);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    emit SendReq2Device(Sendata);

    if(0 == mode)
    {
        QMessageBox::information(NULL, "info", "set weather mode ok", QMessageBox::Yes, QMessageBox::NoButton);
    }
    else
    {
        QMessageBox::information(NULL, "info", "set host mode ok", QMessageBox::Yes, QMessageBox::NoButton);
    }

    mode++;
    mode = mode%2;
}
static void MessageBoxShow(QString str)
{
    QMessageBox::warning(NULL, "warning", str, QMessageBox::Yes, QMessageBox::NoButton);
}
void OTA::RspDataProcess(QByteArray Data)
{
    uint8_t *data = (uint8_t *)Data.data();
    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)data;
    char format_data[64];
    if(MSG_FRAME_HEADER != msg->header)
    {
        sprintf(format_data,"header err=%d",msg->header);
        QString date_str(format_data);
        MessageBoxShow(date_str);
        return;
    }

    switch(msg->cmd)
    {
        case Reset_Cmd:
        {
            uint32_t cal_sum = CalCheckSum(data,sizeof(cmd_msg_frame_t) + 1);
            uint32_t read_sum = *((uint32_t*)(data + sizeof(cmd_msg_frame_t) + 1));
            if(cal_sum != read_sum)
            {
                sprintf(format_data,"frame check err,cal=%d read=%d",cal_sum,read_sum);
                QString date_str(format_data);
                MessageBoxShow(date_str);
                return;
            }
            QMessageBox::information(NULL, "info", "reset ok", QMessageBox::Yes, QMessageBox::NoButton);
        }
        break;
        case CONNECT_CMD:
        {
            uint32_t cal_sum = CalCheckSum(data,sizeof(cmd_msg_frame_t) + 1);
            uint32_t read_sum = *((uint32_t*)(data + sizeof(cmd_msg_frame_t) + 1));
            if(cal_sum != read_sum)
            {
                sprintf(format_data,"frame check err,cal=%d read=%d",cal_sum,read_sum);
                QString date_str(format_data);
                MessageBoxShow(date_str);
                return;
            }
            QMessageBox::information(NULL, "info", "connect ok", QMessageBox::Yes, QMessageBox::NoButton);
        }
        break;
        case JUMP_CMD:
        {
            uint32_t cal_sum = CalCheckSum(data,sizeof(cmd_msg_frame_t) + 1);
            uint32_t read_sum = *((uint32_t*)(data + sizeof(cmd_msg_frame_t) + 1));
            if(cal_sum != read_sum)
            {
                sprintf(format_data,"frame check err,cal=%d read=%d",cal_sum,read_sum);
                QString date_str(format_data);
                MessageBoxShow(date_str);
                return;
            }
            QMessageBox::information(NULL, "info", "jump ok", QMessageBox::Yes, QMessageBox::NoButton);
        }
        break;
        case START_UPDATE:
        {
            set_ota_transmit_state(OTA_TRANSMIT_DATA);
            emit ShowSystemMessage("rsp start update",1500);
        }
        break;
        case UPDATE_DATA:
        {
            if(ota_info_manager.curr_package_num < ota_info_manager.package_num)
            {
                set_ota_transmit_state(OTA_TRANSMIT_DATA);
            }
            else
            {
                 set_ota_transmit_state(OTA_TRANSMIT_END );
            }
        }
        break;
        case UPDATE_END:
        {
            emit ShowSystemMessage("rsp end update",1500);
            QMessageBox::information(NULL, "info", "update ok", QMessageBox::Yes, QMessageBox::NoButton);
        }
        break;
        case VERSION_CMD:
        {
            uint32_t cal_sum = CalCheckSum(data,sizeof(cmd_msg_frame_t) + sizeof(version_info_t));
            uint32_t read_sum = *((uint32_t*)(data + sizeof(cmd_msg_frame_t) + sizeof(version_info_t)));
            if(cal_sum != read_sum)
            {
                sprintf(format_data,"frame check err,cal=%d read=%d",cal_sum,read_sum);
                QString date_str(format_data);
                MessageBoxShow(date_str);
                return;
            }

            version_info_t* version_info = (version_info_t*)(data+sizeof(cmd_msg_frame_t));
            ui->HardwareVersion->setText(QString::number(version_info->hardware_version,16));
            ui->SoftwareVersion->setText(QString::number(version_info->software_version,16));

            if(OTA_BOOT_ATTR == version_info->ota_attr)
            {
                QMessageBox::information(NULL, "info", "version ok,in boot", QMessageBox::Yes, QMessageBox::NoButton);
            }
            else
            {
                QMessageBox::information(NULL, "info", "jump ok,in app", QMessageBox::Yes, QMessageBox::NoButton);
            }
        }
        break;
        case CONNECT_MODE_CMD:
        {
            uint32_t cal_sum = CalCheckSum(data,sizeof(cmd_msg_frame_t) + 1);
            uint32_t read_sum = *((uint32_t*)(data + sizeof(cmd_msg_frame_t) + 1));
            if(cal_sum != read_sum)
            {
                cout <<"frame check err,cal=" << cal_sum <<"read= "<< read_sum << endl;
                return;
            }
        }break;
        default:
        {
            sprintf(format_data,"invalid cmd");
            QString date_str(format_data);
            MessageBoxShow(date_str);
        }
    }
}

OTA::~OTA()
{
    delete  ota_info_manager.BinBuf;
    delete ui;
}

void OTA::on_ResetDevice_pressed()
{

}

QThreadRun::QThreadRun()
{
    StartFlag = false;
}

QThreadRun::~QThreadRun()
{

}

void QThreadRun::SetSwitch(bool flag)
{
    StartFlag = flag;
}

void QThreadRun::SetCallBackFunc(ThreadCallback Func,void* Arg)
{
    CallBack = Func;
    CallBackArg = Arg;
}

void QThreadRun::run()
{
    while(StartFlag)
    {
        emit RunFunc();
        msleep(100);
    }
}
