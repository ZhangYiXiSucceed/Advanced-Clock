#include <QFileDialog>
#include <QFile>
#include <QDateTime>
#include <QMessageBox>
#include "PictureShow.h"
#include "ui_PictureShow.h"

PictureShow::PictureShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureShow)
{
    ui->setupUi(this);

    MyPictureShowTimer = new QTimer;
    picture_info_manager.BinBuf = new quint8[64*1024*1024];

    InitUI();
    InitConnect();
}

PictureShow::~PictureShow()
{
    delete ui;
}

void PictureShow::InitUI()
{
    QGraphicsScene *scene = new QGraphicsScene;
    ui->PictureShowStatic->setScene(scene);
    ui->PictureShowStatic->show();
    scene->addPixmap(BadApplePicture);
}

void PictureShow::InitConnect()
{
    connect(ui->StartTransfer,SIGNAL(clicked(bool)),this,SLOT(StartTransferOLEDShow()));
    connect(ui->SelectShowBinFile,SIGNAL(clicked(bool)),this,SLOT(SelectPictureShowBin()));
    connect(ui->OLEDModeShowSet,SIGNAL(clicked(bool)),this,SLOT(SetOLEDShowMode()));

    connect(MyPictureShowTimer,SIGNAL(timeout()),this,SLOT(PictureUpdate()));

}

void PictureShow::SelectPictureShowBin()
{
    picture_info_manager.FileAddress = QFileDialog::getOpenFileName(this,tr("Open File"),picture_info_manager.FileAddress,tr("(*.bin);;All Files(*.*)"));
    if(picture_info_manager.FileAddress != NULL)
    {
        QFileInfo *Temp = new QFileInfo(picture_info_manager.FileAddress);
        picture_info_manager.BinSize = Temp->size();
        //添加日期等
        QFile *BinFile = new QFile(picture_info_manager.FileAddress);
        if(BinFile->open(QIODevice::ReadOnly))
        {
            emit ShowSystemMessage(tr("打开文件成功，并获取相关信息！"),1500);
            QByteArray  array = BinFile->readAll();
            memcpy(picture_info_manager.BinBuf, array.data(), picture_info_manager.BinSize);
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


void PictureShow::StartTransferOLEDShow()
{
    if(MyPictureShowTimer->isActive())
    {
        MyPictureShowTimer->stop();
        ui->StartTransfer->setText("开始显示");
    }
    else
    {
        MyPictureShowTimer->start(100);
        ui->StartTransfer->setText("停止显示");
    }
}

QImage Binaryzation(uint8_t* buf,uint32_t cnt)
{
    int width = 128;
    int height = 64;
    QImage newImg = QImage(width, height, QImage::Format_RGB888);
    int newGray;
    for (int x = 0; x <height/8 ; x++) {
        for (int y = 0; y <width; y++) {
            uint8_t temp_data=0x01;
            for(int z = 0; z < 8; z++) {
                temp_data = temp_data<<z;
                if (buf[1024*cnt + x*128+y] &  temp_data)
                {
                    newGray = 0;
                }
                else
                {
                    newGray = 255;
                }
                newImg.setPixel(y, x*8+z, qRgb(newGray, newGray, newGray));
            }
        }
    }
    return newImg;
}

void PictureShow::TransmitBinData(uint16_t cnt)
{
    uint8_t buf[sizeof(cmd_msg_frame_t) +  OTA_ONE_PACKAGE_SIZE + sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + OTA_ONE_PACKAGE_SIZE + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = PICTURE_CMD;
    msg->seq = cnt;
    msg->data_len = OTA_ONE_PACKAGE_SIZE;

    uint8_t *data =(uint8_t *)(msg+1);
    memcpy(data,picture_info_manager.BinBuf+cnt*OTA_ONE_PACKAGE_SIZE,OTA_ONE_PACKAGE_SIZE);

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t) + OTA_ONE_PACKAGE_SIZE);
    uint32_t *check_sum = (uint32_t *)(&buf[sizeof(cmd_msg_frame_t) + OTA_ONE_PACKAGE_SIZE + 1]);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    emit SendReq2Device(Sendata);
}

void PictureShow::PictureUpdate()
{
    static int i=0;
    QImage disImage = Binaryzation(picture_info_manager.BinBuf,i);
    QGraphicsScene *scene = new QGraphicsScene;
    ui->OLEDShow->setScene(scene);
    ui->OLEDShow->show();
    scene->addPixmap(QPixmap::fromImage(disImage));
    i++;

    TransmitBinData(i);
}

void PictureShow::SetOLEDShowMode()
{
    static uint8_t mode = 0;
    uint8_t buf[sizeof(cmd_msg_frame_t) +  sizeof(oled_show_mode_set_t) + sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + sizeof(oled_show_mode_set_t) + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = OLED_SHOW_MODE_CMD;
    msg->seq = 0x00;
    msg->data_len = sizeof(connect_mode_set_t);

    oled_show_mode_set_t *rsp = (oled_show_mode_set_t *)(msg + 1);
    rsp->mode = mode;

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t) + sizeof(oled_show_mode_set_t));
    uint32_t *check_sum = (uint32_t *)(rsp+1);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    emit SendReq2Device(Sendata);

    if(0 == mode)
    {
        QMessageBox::information(NULL, "info", "set time mode ok", QMessageBox::Yes, QMessageBox::NoButton);
    }
    else
    {
        QMessageBox::information(NULL, "info", "set connect mode ok", QMessageBox::Yes, QMessageBox::NoButton);
    }

    mode++;
    mode = mode%2;
}
