#include "remosen.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    remosen w;
    w.show();
    return a.exec();
}
