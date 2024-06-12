#include "image_manipulation.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    image_Manipulation window;
    window.show();
    return a.exec();
}
