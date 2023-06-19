#ifndef TIMESHOW_H
#define TIMESHOW_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QTimer>
#include "Cmd.h"

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
    void SetTimeReq(int hour,int minute,int second);
private slots:
    void ScanInternet();
    void CloseInternetConnect();
    void NewConnect();
    void ReadData();
    void disconnectedSlot();
    void WriteTestData();
    void ConnectCmd();
    void SendData2Device(QByteArray Data);
    void SetDate(int year,int month,int day,int week);
    void SetTime(int hour,int minute,int second);
    void TimerUpdate();
private:
    Ui::TimeShow *ui;
    QTcpServer *MyTcpServer;
    QList<QTcpSocket*> MyTcpClient;
    QTcpSocket *currentClient;

    QTimer  *MyTimeShowTimer;

    int InternetPort;
    QString ConnectIP;
    heart_data_t weather_and_time_data_g;
};

#endif // TIMESHOW_H
