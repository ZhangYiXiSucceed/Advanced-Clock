#ifndef OTA_H
#define OTA_H

#include <QWidget>
#include <QTimer>
#include <QThread>
#include <windows.h>
#include "Cmd.h"
#define  OTA_BIN_SIZE   0x20000
#define  OTA_ONE_PACKAGE_SIZE   1024

typedef void (*ThreadCallback)(void*);
class QThreadRun : public QThread
{
    Q_OBJECT

public:
    QThreadRun();
    ~QThreadRun();

    void SetSwitch(bool Flag);
    void SetCallBackFunc(ThreadCallback Func,void* Arg);
    void run();
signals:
    void RunFunc();

private:
    bool StartFlag;
    ThreadCallback CallBack;
    void* CallBackArg;
};

typedef enum OTATransmitStat_enum
{
    START_OTA_TRNASMIT_INFO=0x1,
    START_OTA_TRNASMIT_INFO_RSP,
    OTA_TRANSMIT_DATA,
    OTA_TRANSMIT_DATA_RSP,
    OTA_TRANSMIT_END,
    OTA_TRANSMIT_END_RSP,
}OTATransmitState_t;

typedef struct OTAInfoManagerStruct
{
    QString FileAddress;          //the address of upgrade bin file
    quint64 BinSize;              //the size of bin file
    quint8  *BinBuf;
    quint32 CheckSum;
    quint16 PackageNum;
    quint16 CurrPackageNum;
    OTATransmitState_t State;
}ota_info_manager_t;


namespace Ui {
class OTA;
}
\
void PrintMsg();
class OTA : public QWidget
{
    Q_OBJECT

public:
    explicit OTA(QWidget *parent = nullptr);
    ~OTA();
    void InitUI();
    void InitConnect();

    void TransmitBinInfo();
    void TransmitBinData(uint16_t cnt);
    void TransmitBinEnd();

    void SetOTATransmitState(OTATransmitState_t State);
    OTATransmitState_t GetOTATransmitState();

    static DWORD __stdcall StartSendBinSignal(void* arg)
    {
        OTA *op = (OTA*) arg;
        emit op->SendBinReq();
        QThread::msleep(1000);
        return 0;
    }
    void StartSendBinThread()
    {
        CreateThread(NULL, 0, StartSendBinSignal, this, 0, NULL);
    }

signals:
    void SendReq2Device(QByteArray Data);
    void OpenDeviceReq();
    void CloseDeviceReq();
    void ShowParameter(QString, QString);
    void ShowSystemMessage(QString,uint16_t);
    void SendBinReq();

private slots:
    void RspDataProcess(QByteArray Data);
    void on_ResetDevice_pressed();
    void ResetDeviceCmd();
    void JumpDeviceCmd();
    void ConnectDeviceCmd();
    void OpenDevice();
    void CloseDevice();
    void SelectOTABin();
    void StartUpgrade();
    void UpgradeBinThread();
    void GetVersionCmd();
    void SetConnectMode();
private:
    Ui::OTA *ui;
    ota_info_manager_t  OTAInfoManager;

    QTimer  *MyStartConnectTimer;
    QThreadRun *MyThread;

    QPixmap DeviceClosePic=tr(":/Image/Close.png");
    QPixmap DeviceOpenPic=tr(":/Image/Open.png");

    QPixmap DeviceConnectPic=tr(":/Image/Connect.png");
    QPixmap DeviceJumpPic=tr(":/Image/Jump.png");

    QPixmap DeviceResetPic=tr(":/Image/Reset.png");
    QPixmap DeviceVerSionPic=tr(":/Image/GetVersion.png");

    QPixmap DeviceSetPic=tr(":/Image/Set.png");
    QPixmap SelectUpgradeFilePic=tr(":/Image/SelectFile.png");
    QPixmap StartUpgradePic=tr(":/Image/Start.png");
};

#endif // OTA_H
