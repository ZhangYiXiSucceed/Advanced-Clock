#ifndef TIMESHOW_H
#define TIMESHOW_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>

namespace Ui {
class TimeShow;
}

class TimeShow : public QWidget
{
    Q_OBJECT

public:
    explicit TimeShow(QWidget *parent = nullptr);
    ~TimeShow();
    void InitUI();
    void InitConnect();
    void RspDataProcess(QByteArray buf);
    void HeartCmdRsp();

signals:
    void ShowParameter(QString, QString);
    void ShowSystemMessage(QString,uint16_t);
    void SendData2OTA(QByteArray Data);
private slots:
    void ScanInternet();
    void CloseInternetConnect();
    void NewConnect();
    void ReadData();
    void disconnectedSlot();
    void WriteTestData();
    void ConnectCmd();
    void SendData2Device(QByteArray Data);
private:
    Ui::TimeShow *ui;
    QTcpServer *MyTcpServer;
    QList<QTcpSocket*> MyTcpClient;
    QTcpSocket *currentClient;

    int InternetPort;
    QString ConnectIP;
};

#endif // TIMESHOW_H
