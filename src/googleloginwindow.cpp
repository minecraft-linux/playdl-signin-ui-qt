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
#include <QLabel>
#include <QDesktopServices>
#include <QPushButton>
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
    
    {
        auto label = new QLabel(tr("Failed to load the <a href=\"%1\">Google login Page</a>, please check your Internet connection.").arg(GoogleLoginWindow::DEFAULT_URL), this);
        label->setWordWrap(true);
        connect(label, &QLabel::linkActivated, this, [](QString link) {
            QDesktopServices::openUrl(QUrl(link));
        });
        failedLoadingPage = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(failedLoadingPage);
        layout->addWidget(label);
        auto tryagain = new QPushButton("Try again");
        connect(tryagain, &QPushButton::clicked, this, [this]() {
            this->stacked->setCurrentWidget(this->loadingIndicatorCtr);
            this->webView->setUrl(QUrl(GoogleLoginWindow::DEFAULT_URL));
        });
        layout->addWidget(tryagain);
        stacked->addWidget(failedLoadingPage);
    }
    {
        auto label = new QLabel(tr("Warning this Launcher ( qtwebengine ) cannot verify the certificate of the <a href=\"%1\">Google login Page</a>.<br/>It is insecure to connect and providing credentials to a possible hijacked login page.<br/>Otherwise your certificate store may be outdated or incompatible with this Launcher release.").arg(GoogleLoginWindow::DEFAULT_URL), this);
        label->setWordWrap(true);
        connect(label, &QLabel::linkActivated, this, [](QString link) {
            QDesktopServices::openUrl(QUrl(link));
        });
        certErrorPage = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(certErrorPage);
        layout->addWidget(label);
        auto ignoreCert = new QPushButton(tr("Go ahead and ignore this error at your own risk"));
        connect(ignoreCert, &QPushButton::clicked, this, [this]() {
            this->certificateError = false;
            this->ignoreCertificateError = true;
            this->stacked->setCurrentWidget(this->loadingIndicatorCtr);
            this->webView->setUrl(QUrl(GoogleLoginWindow::DEFAULT_URL));
        });
        layout->addWidget(ignoreCert);
        stacked->addWidget(certErrorPage);
    }
    {
        webviewPage = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(webviewPage);
        webView = new QWebEngineView(this);
        auto label = new QLabel(tr("Information about Security: Google identifies this Launcher as a Samsung Galaxy S8, your credentials aren't send to such a device. This Launcher is not Google Play Certified. This Launcher stores your Google Play access token unencrypted on your disk."), this);
        label->setWordWrap(true);
        layout->addWidget(label);
        layout->setStretchFactor(label, 0);
        layout->addWidget(webView);
        layout->setStretchFactor(webView, 1);
    }
    auto webPage = new WebPage(this);
    connect(webPage, &WebPage::verifyCertificateError, this, [this](QString url, QString errormsg) {
        this->certificateError = true;
        return this->ignoreCertificateError;
    });
    webView->setPage(webPage);
    stacked->addWidget(webviewPage);
    setupWebBrowser();
    webView->setUrl(QUrl(DEFAULT_URL));

    setLayout(stacked);
}

void GoogleLoginWindow::setupWebBrowser() {
    connect(webView, &QWebEngineView::urlChanged, this, &GoogleLoginWindow::onUrlChanged);
    connect(webView, &QWebEngineView::loadFinished, this, &GoogleLoginWindow::onLoadFinished);

    injectWebScripts();

    QWebChannel* channel = new QWebChannel(webView);
    GoogleLoginWindowApi* api = new GoogleLoginWindowApi(this, webView);
    channel->registerObject(QStringLiteral("loginWindow"), api);
    webView->page()->setWebChannel(channel);

    QWebEngineCookieStore* cookies = webView->page()->profile()->cookieStore();

    cookies->connect(cookies, &QWebEngineCookieStore::cookieAdded, this, &GoogleLoginWindow::onCookieAdded);
    cookies->deleteAllCookies();

    webView->page()->profile()->setHttpUserAgent("Mozilla/5.0 (Linux; Android 7.0; SM-G892A Build/NRD90M; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/60.0.3112.107 Mobile Safari/537.36");

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

void GoogleLoginWindow::onLoadFinished(bool ok) {
    if (!ok) {
        stacked->setCurrentWidget((this->certificateError && !this->ignoreCertificateError) ? certErrorPage : failedLoadingPage);
    }
}

void GoogleLoginWindow::onCookieAdded(const QNetworkCookie &cookie) {
    if (cookie.name() == "oauth_token")
        pAccountToken = cookie.value();
    else if (cookie.name() == "user_id")
        pAccountUserId = cookie.value();
}

void GoogleLoginWindow::showWebBrowser() {
    stacked->setCurrentWidget(webviewPage);
}

bool WebPage::certificateError(const QWebEngineCertificateError& err) {
    return emit verifyCertificateError(err.url().toString(), err.errorDescription());
}