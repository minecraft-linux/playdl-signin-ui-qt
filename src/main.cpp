#include <QApplication>
#include <googleloginwindow.h>

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(googlesigninui);
    QApplication app(argc, argv);
    GoogleLoginWindow window;
    window.show();
    return app.exec();
}
