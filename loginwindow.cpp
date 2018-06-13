#include "loginwindow.h"

const char* const LoginWindow::DEFAULT_URL = "https://accounts.google.com/embedded/setup/v2/android?source=com.android.settings&xoauth_display_name=Android%20Phone&canFrp=1&canSk=1&lang=en&langCountry=en_us&hl=en-US&cc=us";

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent) {
    resize(480, 640);

    setWindowFlag(Qt::Dialog);

    webView.setParent(this);
    webView.setUrl(QUrl(DEFAULT_URL));
    setCentralWidget(&webView);
}
