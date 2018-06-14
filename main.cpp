#include <QApplication>
#include "loginwindow.h"

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(googlesigninui);
    QApplication app(argc, argv);
    LoginWindow window;
    window.show();
    return app.exec();
}
