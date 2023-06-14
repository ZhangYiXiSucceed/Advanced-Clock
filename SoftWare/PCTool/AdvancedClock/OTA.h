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
private slots:
    void RspDataProcess(QByteArray Data);
    void on_ResetDevice_pressed();
    void ResetDeviceCmd();

private:
    Ui::OTA *ui;
};

#endif // OTA_H
