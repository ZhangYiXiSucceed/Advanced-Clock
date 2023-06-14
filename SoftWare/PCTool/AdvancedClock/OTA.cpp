#include "OTA.h"
#include "ui_OTA.h"
#include "Cmd.h"
#include <QMessageBox>
#include <iostream>
#include <iomanip>
using namespace std;

OTA::OTA(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OTA)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();
}

void OTA::InitUI()
{

}
void OTA::InitConnect()
{
    connect(ui->ResetDevice,SIGNAL(clicked(bool)),this,SLOT(ResetDeviceCmd()));
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
            QMessageBox::information(NULL, "info", "connect ok", QMessageBox::Yes, QMessageBox::NoButton);
        }
        break;
        default:
        {

        }
    }
}

OTA::~OTA()
{
    delete ui;
}

void OTA::on_ResetDevice_pressed()
{

}

