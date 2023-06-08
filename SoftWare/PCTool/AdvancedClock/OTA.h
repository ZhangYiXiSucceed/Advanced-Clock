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

private:
    Ui::OTA *ui;
};

#endif // OTA_H
