#include <QApplication>
#include <QTextStream>
#include <googleloginwindow.h>

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(googlesigninui);
    QApplication app(argc, argv);
    GoogleLoginWindow window;
    QObject::connect(&window, &QDialog::accepted, [&window]() {
        QTextStream stream(stdout);
        stream << "user_email = " << window.accountIdentifier() << "\n";
        stream << "user_id = " << window.accountUserId() << "\n";
        stream << "user_token = " << window.accountToken() << "\n";
    });
    window.show();
    return app.exec();
}
