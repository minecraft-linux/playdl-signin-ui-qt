#include <googleloginwindow.h>
#include <QDebug>
#include <QFile>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QWebEngineCookieStore>
#include <QWebEngineProfile>
#include <QWebChannel>
#include "materialbusyindicator.h"

const char* const GoogleLoginWindow::DEFAULT_URL = "https://accounts.google.com/embedded/setup/v2/android?source=com.android.settings&xoauth_display_name=Android%20Phone&canFrp=1&canSk=1&lang=en&langCountry=en_us&hl=en-US&cc=us";

GoogleLoginWindow::GoogleLoginWindow(QWidget *parent) : QDialog(parent) {
    setWindowFlag(Qt::Dialog);
    setWindowTitle("Google Sign-In");
    resize(480, 640);

    stacked = new QStackedLayout(this);

    loadingIndicatorCtr = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(loadingIndicatorCtr);
    MaterialBusyIndicator* loadingIndicator = new MaterialBusyIndicator(loadingIndicatorCtr);
    loadingIndicator->setFixedSize(48, 48);
    layout->addWidget(loadingIndicator, 0, Qt::AlignCenter);
    stacked->addWidget(loadingIndicatorCtr);

    webView = new QWebEngineView(this);
    stacked->addWidget(webView);
    setupWebBrowser();
    webView->setUrl(QUrl(DEFAULT_URL));

    setLayout(stacked);
}

void GoogleLoginWindow::setupWebBrowser() {
    connect(webView, &QWebEngineView::urlChanged, this, &GoogleLoginWindow::onUrlChanged);

    injectWebScripts();

    QWebChannel* channel = new QWebChannel(webView);
    GoogleLoginWindowApi* api = new GoogleLoginWindowApi(this, webView);
    channel->registerObject(QStringLiteral("loginWindow"), api);
    webView->page()->setWebChannel(channel);

    QWebEngineCookieStore* cookies = webView->page()->profile()->cookieStore();
    cookies->connect(cookies, &QWebEngineCookieStore::cookieAdded, this, &GoogleLoginWindow::onCookieAdded);
    cookies->deleteAllCookies();
}

void GoogleLoginWindow::injectWebScripts() {
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
        file.setFileName(":/src/injectjs.js");
        file.open(QIODevice::ReadOnly);
        source += file.readAll();
    }
    script.setSourceCode(source);
    webView->page()->scripts().insert(script);
}

void GoogleLoginWindow::onUrlChanged(const QUrl &url) {
    if (url.fragment() == "close")
        accept();
}

void GoogleLoginWindow::onCookieAdded(const QNetworkCookie &cookie) {
    if (cookie.name() == "oauth_token")
        pAccountToken = cookie.value();
    else if (cookie.name() == "user_id")
        pAccountUserId = cookie.value();
}

void GoogleLoginWindow::showWebBrowser() {
    stacked->setCurrentWidget(webView);
}
