#ifndef PICTURESHOW_H
#define PICTURESHOW_H

#include "OTA.h"
#include <QWidget>
#include <QTimer>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QMediaPlayer>
#include <QApplication>

typedef struct picture_info_manager_struct
{
    QString FileAddress;          //the address of upgrade bin file
    quint64 BinSize;              //the size of bin file
    quint8  *BinBuf;
    OTATransmitState_t state;
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

    QGraphicsScene *scene;
    QGraphicsPixmapItem  *item;
    QMediaPlayer *player;

    QPixmap BadApplePicture=tr(":/Image/badapple.png");
    QPixmap StartShow=tr(":/Image/Start.png");
    QPixmap StopShow=tr(":/Image/Stop.png");
    QPixmap SelectShowFile=tr(":/Image/SelectFile.png");
    QPixmap ModeSet=tr(":/Image/ModeSet.png");
};

#endif // PICTURESHOW_H
