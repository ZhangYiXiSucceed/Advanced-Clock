#ifndef CONTROLDEVICE_H
#define CONTROLDEVICE_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>

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
    void GetUSBInfo(uint8_t data_type);
private slots:
    void ScanInternet();
    void CloseInternetConnect();
    void NewConnect();
    void ReadData();
    void disconnectedSlot();
private:
    QTcpServer *MyTcpServer;
    QList<QTcpSocket*> MyTcpClient;
    QTcpSocket *currentClient;

    int InternetPort;
    QString ConnectIP;
    Ui::ControlDevice *ui;
};

#endif // CONTROLDEVICE_H
