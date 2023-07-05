#ifndef PICTURESHOW_H
#define PICTURESHOW_H

#include "OTA.h"
#include <QWidget>
#include <QTimer>

typedef struct picture_info_manager_struct
{
    QString FileAddress;          //the address of upgrade bin file
    quint64 BinSize;              //the size of bin file
    quint8  *BinBuf;
    ota_transmit_state_t state;
}picture_info_manager_t;

namespace Ui {
class PictureShow;
}

class PictureShow : public QWidget
{
    Q_OBJECT

public:
    explicit PictureShow(QWidget *parent = nullptr);
    ~PictureShow();
    void InitUI();
    void InitConnect();
    void TransmitBinData(uint16_t cnt);
signals:
    void SendReq2Device(QByteArray Data);
    void ShowParameter(QString, QString);
    void ShowSystemMessage(QString,uint16_t);

private slots:
    void PictureUpdate();
    void SelectPictureShowBin();
    void StartTransferOLEDShow();
    void SetOLEDShowMode();
private:
    Ui::PictureShow *ui;
    QTimer  *MyPictureShowTimer;
    picture_info_manager_t picture_info_manager;

    QPixmap BadApplePicture=tr(":/Image/badapple.png");
};

#endif // PICTURESHOW_H
