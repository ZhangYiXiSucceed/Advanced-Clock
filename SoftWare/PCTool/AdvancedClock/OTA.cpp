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
    BinBuf = new quint8[1024];

    InitUI();
    InitConnect();
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

    connect(MyStartConnectTimer,SIGNAL(timeout()),this,SLOT(OpenDevice()));
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
    FileAddress = QFileDialog::getOpenFileName(this,tr("Open File"),FileAddress,tr("(*.bin);;(*.hex);;(*.axf);;(*.txt);;All Files(*.*)"));
    ui->UpgradBinFileAddress->setText(FileAddress);
    if(FileAddress != NULL)
    {
        QFileInfo *Temp = new QFileInfo(FileAddress);
        BinSize = Temp->size();
        //添加日期等
        QFile *BinFile = new QFile(FileAddress);
        if(BinFile->open(QIODevice::ReadOnly))
        {
            emit ShowSystemMessage(tr("打开文件成功，并获取相关信息！"),1500);
            if(BinSize<2014)
                ui->UpgradBinFileSize->setText(tr("%1B\n").arg(BinSize));
            else if(BinSize<1024*1024)
                ui->UpgradBinFileSize->setText(tr("%1K %2B\n").arg(BinSize/1024).arg(BinSize%1024));
            else
                ui->UpgradBinFileSize->setText(tr("%1M %2K %3B\n").arg(BinSize/1024/1024).arg(BinSize/1024%1024).arg(BinSize%1024));
            BinFile->read(BinBuf,0);
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
    memcpy(data,BinBuf+cnt*OTA_ONE_PACKAGE_SIZE,OTA_ONE_PACKAGE_SIZE);

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t) + OTA_ONE_PACKAGE_SIZE);
    uint32_t *check_sum = (uint32_t *)(&buf[sizeof(cmd_msg_frame_t) + OTA_ONE_PACKAGE_SIZE + 1]);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    emit SendReq2Device(Sendata);
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
    info->bin_size = BinSize;
    info->package_num = (BinSize % OTA_ONE_PACKAGE_SIZE)?(BinSize/OTA_ONE_PACKAGE_SIZE + 1):(BinSize/OTA_ONE_PACKAGE_SIZE);
    info->check_sum = CalCheckSum(BinBuf,BinSize);

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t) + sizeof(ota_package_info_t));
    uint32_t *check_sum = (uint32_t *)(&buf[sizeof(cmd_msg_frame_t) + sizeof(ota_package_info_t) + 1]);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    emit SendReq2Device(Sendata);
}

void OTA::StartUpgrade()
{

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
        default:
        {

        }
    }
}

OTA::~OTA()
{
    delete BinBuf;
    delete ui;
}

void OTA::on_ResetDevice_pressed()
{

}

