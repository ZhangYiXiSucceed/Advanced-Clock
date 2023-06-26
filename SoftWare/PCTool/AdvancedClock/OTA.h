#ifndef OTA_H
#define OTA_H

#include <QWidget>
#include <QTimer>
#include <QThread>
#define  OTA_BIN_SIZE   0x20000
#define  OTA_ONE_PACKAGE_SIZE   64

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

private:
    bool StartFlag;
    ThreadCallback CallBack;
    void* CallBackArg;
};

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
    void TransmitBinData(uint8_t cnt);
    void TransmitBinEnd();


signals:
    void SendReq2Device(QByteArray Data);
    void OpenDeviceReq();
    void CloseDeviceReq();
    void ShowParameter(QString, QString);
    void ShowSystemMessage(QString,uint16_t);

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

private:
    Ui::OTA *ui;
    QString FileAddress;          //the address of upgrade bin file
    quint64 BinSize;              //the size of bin file
    quint8  *BinBuf;

    QTimer  *MyStartConnectTimer;
    QThreadRun *MyThread;

};

#endif // OTA_H
