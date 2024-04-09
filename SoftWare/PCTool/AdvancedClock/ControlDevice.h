#ifndef CONTROLDEVICE_H
#define CONTROLDEVICE_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QTimer>

namespace Ui {
class ControlDevice;
}

class ControlDevice : public QWidget
{
    Q_OBJECT

public:
    explicit ControlDevice(QWidget *parent = nullptr);
    ~ControlDevice();
    void InitUI();
    void InitConnect();
    void RspDataProcess(QByteArray buffer);
    void SendData2Device(QByteArray Data);
private slots:
    void ScanInternet();
    void CloseInternetConnect();
    void NewConnect();
    void ReadData();
    void disconnectedSlot();
    void GetUSBInfoCmdSend();
private:
    QTcpServer *MyTcpServer;
    QList<QTcpSocket*> MyTcpClient;
    QTcpSocket *currentClient;
    QTimer  *GetUSBInfoTimer;

    int InternetPort;
    QString ConnectIP;
    Ui::ControlDevice *ui;
};

#endif // CONTROLDEVICE_H
