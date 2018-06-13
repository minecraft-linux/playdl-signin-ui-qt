#include "loginwindow.h"
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWebEngineView>
#include "materialbusyindicator.h"

const char* const LoginWindow::DEFAULT_URL = "https://accounts.google.com/embedded/setup/v2/android?source=com.android.settings&xoauth_display_name=Android%20Phone&canFrp=1&canSk=1&lang=en&langCountry=en_us&hl=en-US&cc=us";

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowFlag(Qt::Dialog);
    resize(480, 640);

    QStackedWidget* stacked = new QStackedWidget(this);

    QWidget* loadingIndicatorCtr = new QWidget(stacked);
    QVBoxLayout* layout = new QVBoxLayout(loadingIndicatorCtr);
    MaterialBusyIndicator* loadingIndicator = new MaterialBusyIndicator(loadingIndicatorCtr);
    loadingIndicator->setFixedSize(48, 48);
    layout->addWidget(loadingIndicator, 0, Qt::AlignCenter);
    stacked->addWidget(loadingIndicatorCtr);

    QWebEngineView* webView = new QWebEngineView(stacked);
    stacked->addWidget(webView);
    webView->setUrl(QUrl(DEFAULT_URL));

    setCentralWidget(stacked);
}
