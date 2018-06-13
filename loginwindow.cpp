#include "loginwindow.h"
#include <QFile>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QWebChannel>
#include "materialbusyindicator.h"

const char* const LoginWindow::DEFAULT_URL = "https://accounts.google.com/embedded/setup/v2/android?source=com.android.settings&xoauth_display_name=Android%20Phone&canFrp=1&canSk=1&lang=en&langCountry=en_us&hl=en-US&cc=us";

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowFlag(Qt::Dialog);
    resize(480, 640);

    stacked = new QStackedWidget(this);

    loadingIndicatorCtr = new QWidget(stacked);
    QVBoxLayout* layout = new QVBoxLayout(loadingIndicatorCtr);
    MaterialBusyIndicator* loadingIndicator = new MaterialBusyIndicator(loadingIndicatorCtr);
    loadingIndicator->setFixedSize(48, 48);
    layout->addWidget(loadingIndicator, 0, Qt::AlignCenter);
    stacked->addWidget(loadingIndicatorCtr);

    webView = new QWebEngineView(stacked);
    stacked->addWidget(webView);
    setupWebBrowser();
    webView->setUrl(QUrl(DEFAULT_URL));

    setCentralWidget(stacked);
}

void LoginWindow::setupWebBrowser() {
    injectWebScripts();

    QWebChannel* channel = new QWebChannel(webView);
    LoginWindowApi* api = new LoginWindowApi(this, webView);
    channel->registerObject(QStringLiteral("loginWindow"), api);
    webView->page()->setWebChannel(channel);
}

void LoginWindow::injectWebScripts() {
    QWebEngineScript script;
    script.setInjectionPoint(QWebEngineScript::DocumentCreation);
    script.setWorldId(QWebEngineScript::MainWorld);
    script.setName("SignInScript");
    QString source;
    {
        QFile file;
        file.setFileName(":/qtwebchannel/qwebchannel.js");
        file.open(QIODevice::ReadOnly);
        source += file.readAll();
    }
    {
        QFile file;
        file.setFileName(":/injectjs.js");
        file.open(QIODevice::ReadOnly);
        source += file.readAll();
    }
    script.setSourceCode(source);
    webView->page()->scripts().insert(script);
}

void LoginWindow::showWebBrowser() {
    stacked->setCurrentWidget(webView);
}
