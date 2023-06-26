#ifndef OTA_H
#define OTA_H

#include <QWidget>
#include <QTimer>
#define  OTA_BIN_SIZE   0x20000
#define  OTA_ONE_PACKAGE_SIZE   64
namespace Ui {
class OTA;
}

class OTA : public QWidget
{
    Q_OBJECT

public:
    explicit OTA(QWidget *parent = nullptr);
    ~OTA();
    void InitUI();
    void InitConnect();
    void TransmitBinData(uint8_t cnt);

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
    void TransmitBinInfo();
private:
    Ui::OTA *ui;
    QString FileAddress;          //the address of upgrade bin file
    quint64 BinSize;              //the size of bin file
    quint8  *BinBuf;

    QTimer  *MyStartConnectTimer;
};

#endif // OTA_H
