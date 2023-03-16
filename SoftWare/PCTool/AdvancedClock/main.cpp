#include "AdvancedClock.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AdvancedClock w;
    w.show();
    return a.exec();
}
