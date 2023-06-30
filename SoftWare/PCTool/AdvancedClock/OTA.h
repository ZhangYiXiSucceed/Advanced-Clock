#ifndef OTA_H
#define OTA_H

#include <QWidget>
#include <QTimer>
#include <QThread>
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

typedef enum ota_transmit_state_enum
{
    START_OTA_TRNASMIT_INFO=0x1,
    START_OTA_TRNASMIT_INFO_RSP,
    OTA_TRANSMIT_DATA,
    OTA_TRANSMIT_DATA_RSP,
    OTA_TRANSMIT_END,
    OTA_TRANSMIT_END_RSP,
}ota_transmit_state_t;

typedef struct ota_info_manager_struct
{
    QString FileAddress;          //the address of upgrade bin file
    quint64 BinSize;              //the size of bin file
    quint8  *BinBuf;
    quint32 check_sum;
    quint16 package_num;
    quint16 curr_package_num;
    ota_transmit_state_t state;
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

    void set_ota_transmit_state(ota_transmit_state_t state);
    ota_transmit_state_t get_ota_transmit_state();
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
    void UpgradeBinThread();

private:
    Ui::OTA *ui;
    ota_info_manager_t  ota_info_manager;

    QTimer  *MyStartConnectTimer;
    QThreadRun *MyThread;

};

#endif // OTA_H
