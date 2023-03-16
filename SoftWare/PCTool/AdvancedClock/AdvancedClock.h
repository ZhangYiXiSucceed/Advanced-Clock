#ifndef ADVANCEDCLOCK_H
#define ADVANCEDCLOCK_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class AdvancedClock; }
QT_END_NAMESPACE

class AdvancedClock : public QMainWindow
{
    Q_OBJECT

public:
    AdvancedClock(QWidget *parent = nullptr);
    ~AdvancedClock();

private:
    Ui::AdvancedClock *ui;
};
#endif // ADVANCEDCLOCK_H
