#include "OTA.h"
#include "ui_OTA.h"
#include "Cmd.h"
#include <QMessageBox>
#include <iostream>
#include <iomanip>
#include <QFileDialog>
#include <QFile>
using namespace std;

OTA::OTA(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OTA)
{
    ui->setupUi(this);
    MyStartConnectTimer = new QTimer;
    MyStartConnectTimer->start(5000);
    MyStartConnectTimer->setSingleShot(true);

    ota_info_manager.BinBuf = new quint8[1024];
    ota_info_manager.state = START_OTA_TRNASMIT_INFO;

    MyThread = new QThreadRun;

    MyThread->SetSwitch(true);
    MyThread->SetCallBackFunc((ThreadCallback)&PrintMsg,NULL);

    InitUI();
    InitConnect();
    PrintMsg();
}

void OTA::InitUI()
{
    ui->OpenDevice->setEnabled(true);
    ui->CloseDevice->setEnabled(false);
}
void OTA::InitConnect()
{
    connect(ui->ResetDevice,SIGNAL(clicked(bool)),this,SLOT(ResetDeviceCmd()));
    connect(ui->ConnectDevice,SIGNAL(clicked(bool)),this,SLOT(ConnectDeviceCmd()));
    connect(ui->JumpDevice,SIGNAL(clicked(bool)),this,SLOT(JumpDeviceCmd()));

    connect(ui->OpenDevice,SIGNAL(clicked(bool)),this,SLOT(OpenDevice()));
    connect(ui->CloseDevice,SIGNAL(clicked(bool)),this,SLOT(CloseDevice()));

    connect(ui->SelectUpgradBinFile,SIGNAL(clicked(bool)),this,SLOT(SelectOTABin()));
    connect(ui->StartUpgrade,SIGNAL(clicked(bool)),this,SLOT(StartUpgrade()));

    connect(MyStartConnectTimer,SIGNAL(timeout()),this,SLOT(OpenDevice()));
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

void OTA::UpgradeBinThread()
{
    switch(ota_info_manager.state)
    {
        case START_OTA_TRNASMIT_INFO:
        {
            TransmitBinInfo();
            set_ota_transmit_state(START_OTA_TRNASMIT_INFO_RSP);
            emit ShowSystemMessage(tr(" start ota transmit info"),1500);
        }
        break;
        case START_OTA_TRNASMIT_INFO_RSP:
        {
            QThread::msleep(1000);
        }
        break;
        case OTA_TRANSMIT_DATA:
        {
            if(ota_info_manager.curr_package_num < ota_info_manager.package_num)
            {
                TransmitBinData(ota_info_manager.curr_package_num);
                set_ota_transmit_state(OTA_TRANSMIT_DATA_RSP);
            }
            else
            {
                set_ota_transmit_state(OTA_TRANSMIT_END);
            }
        }
        break;
        case OTA_TRANSMIT_DATA_RSP:
        {
            QThread::msleep(1000);
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
            QThread::msleep(1000);
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
    ota_info_manager.FileAddress = QFileDialog::getOpenFileName(this,tr("Open File"),FileAddress,tr("(*.bin);;(*.hex);;(*.axf);;(*.txt);;All Files(*.*)"));
    ui->UpgradBinFileAddress->setText(ota_info_manager.FileAddress);
    if(ota_info_manager.FileAddress != NULL)
    {
        QFileInfo *Temp = new QFileInfo(ota_info_manager.FileAddress);
        ota_info_manager.BinSize = Temp->size();
        //添加日期等
        QFile *BinFile = new QFile(ota_info_manager.FileAddress);
        if(BinFile->open(QIODevice::ReadOnly))
        {
            emit ShowSystemMessage(tr("打开文件成功，并获取相关信息！"),1500);
            if(ota_info_manager.BinSize<2014)
                ui->UpgradBinFileSize->setText(tr("%1B\n").arg(ota_info_manager.BinSize));
            else if(ota_info_manager.BinSize<1024*1024)
                ui->UpgradBinFileSize->setText(tr("%1K %2B\n").arg(ota_info_manager.BinSize/1024).arg(ota_info_manager.BinSize%1024));
            else
                ui->UpgradBinFileSize->setText(tr("%1M %2K %3B\n").arg(ota_info_manager.BinSize/1024/1024).arg(ota_info_manager.BinSize/1024%1024).arg(ota_info_manager.BinSize%1024));
            BinFile->read((char*)ota_info_manager.BinBuf,0);
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
    uint32_t *check_sum = (uint32_t *)(&buf[sizeof(cmd_msg_frame_t) + sizeof(ota_package_info_t) + 1]);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    emit SendReq2Device(Sendata);
}

void OTA::TransmitBinData(uint8_t cnt)
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
    MyThread->start();
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

void OTA::RspDataProcess(QByteArray Data)
{
    uint8_t *data = (uint8_t *)Data.data();
    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)data;

    if(MSG_FRAME_HEADER != msg->header)
    {
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
                cout <<"frame check err,cal=" << cal_sum <<"read= "<< cal_sum << endl;
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
                cout <<"frame check err,cal=" << cal_sum <<"read= "<< cal_sum << endl;
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
                cout <<"frame check err,cal=" << cal_sum <<"read= "<< cal_sum << endl;
                return;
            }
            QMessageBox::information(NULL, "info", "jump ok", QMessageBox::Yes, QMessageBox::NoButton);
        }
        break;
        case START_UPDATE:
        {

        }
        break;
        case UPDATE_DATA:
        {

        }
        break;
        case UPDATE_END:
        {

        }
        break;
        default:
        {

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
        (*CallBack)(CallBackArg);
    }
}
