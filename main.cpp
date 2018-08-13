#include "tuwidget.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TuWidget w;
    w.show();
    a.setStyle(QStyleFactory::create("fusion"));
    return a.exec();
}
