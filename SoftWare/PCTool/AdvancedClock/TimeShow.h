#ifndef TIMESHOW_H
#define TIMESHOW_H

#include <QWidget>

namespace Ui {
class TimeShow;
}

class TimeShow : public QWidget
{
    Q_OBJECT

public:
    explicit TimeShow(QWidget *parent = nullptr);
    ~TimeShow();

private:
    Ui::TimeShow *ui;
};

#endif // TIMESHOW_H
