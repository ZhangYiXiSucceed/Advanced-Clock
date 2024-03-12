#ifndef ADVANCEDCLOCK_H
#define ADVANCEDCLOCK_H

#include <QMainWindow>
#include <QLabel>
#include <QTextCodec>
#include "TimeShow.h"
#include "PictureShow.h"
#include "OTA.h"
#include "ControlDevice.h"


QT_BEGIN_NAMESPACE
namespace Ui { class AdvancedClock; }
QT_END_NAMESPACE

class AdvancedClock : public QMainWindow
{
    Q_OBJECT

public:
    AdvancedClock(QWidget *parent = nullptr);
    void InitUI();
    void InitConnect();
    ~AdvancedClock();
     QTabWidget *MyTabWidget;

private slots:
     void ShowSystemMessage(QString Message,uint16_t ShowTime);
     void ShowParameter(QString InternetIP,QString Port);
private:

    Ui::AdvancedClock *ui;
    TimeShow *MyTimeShow;
    PictureShow *MyPictureShow;
    OTA *MyOTA;
    ControlDevice *MyControlDevice;

    QLabel *InternetIP;
    QLabel *Port;

    QString TextMessage;
};
#endif // ADVANCEDCLOCK_H
