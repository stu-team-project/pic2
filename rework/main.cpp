#include "rework.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    rework w;
    w.show();
    return a.exec();
}
