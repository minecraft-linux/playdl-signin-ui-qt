#include <QApplication>
#include "loginwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    LoginWindow window;
    window.show();
    return app.exec();
}
