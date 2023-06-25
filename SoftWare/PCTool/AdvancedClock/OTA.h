#ifndef OTA_H
#define OTA_H

#include <QWidget>

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
};

#endif // OTA_H
