#ifndef ADVANCEDCLOCK_H
#define ADVANCEDCLOCK_H

#include <QMainWindow>
#include <TimeShow.h>
#include <PictureShow.h>
#include <OTA.h>

QT_BEGIN_NAMESPACE
namespace Ui { class AdvancedClock; }
QT_END_NAMESPACE

class AdvancedClock : public QMainWindow
{
    Q_OBJECT

public:
    AdvancedClock(QWidget *parent = nullptr);
    void InitUI();
    ~AdvancedClock();
     QTabWidget *MyTabWidget;

private:
    Ui::AdvancedClock *ui;
    TimeShow *MyTimeShow;
    PictureShow *MyPictureShow;
    OTA *MyOTA;
};
#endif // ADVANCEDCLOCK_H
