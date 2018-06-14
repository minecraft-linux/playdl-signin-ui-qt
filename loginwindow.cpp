#include "loginwindow.h"
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

const char* const LoginWindow::DEFAULT_URL = "https://accounts.google.com/embedded/setup/v2/android?source=com.android.settings&xoauth_display_name=Android%20Phone&canFrp=1&canSk=1&lang=en&langCountry=en_us&hl=en-US&cc=us";

LoginWindow::LoginWindow(QWidget *parent) : QDialog(parent) {
    setWindowFlag(Qt::Dialog);
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

void LoginWindow::setupWebBrowser() {
    connect(webView, &QWebEngineView::urlChanged, this, &LoginWindow::onUrlChanged);

    injectWebScripts();

    QWebChannel* channel = new QWebChannel(webView);
    LoginWindowApi* api = new LoginWindowApi(this, webView);
    channel->registerObject(QStringLiteral("loginWindow"), api);
    webView->page()->setWebChannel(channel);

    QWebEngineCookieStore* cookies = webView->page()->profile()->cookieStore();
    cookies->connect(cookies, &QWebEngineCookieStore::cookieAdded, this, &LoginWindow::onCookieAdded);
    cookies->deleteAllCookies();
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

void LoginWindow::onUrlChanged(const QUrl &url) {
    if (url.fragment() == "close") {
        qDebug() << "Auth flow finished";
        qDebug() << "Account" << accountIdentifier;
        qDebug() << "Token" << accountToken;
        qDebug() << "User ID" << accountUserId;
    }
}

void LoginWindow::onCookieAdded(const QNetworkCookie &cookie) {
    if (cookie.name() == "oauth_token")
        accountToken = cookie.value();
    else if (cookie.name() == "user_id")
        accountUserId = cookie.value();
}

void LoginWindow::showWebBrowser() {
    stacked->setCurrentWidget(webView);
}
