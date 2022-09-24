#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#ifndef WITHOUT_WEBENGINE
#include <QWebEnginePage>
#include <QWebEngineCertificateError>
#endif

class QStackedLayout;
class QNetworkCookie;
class QWebEngineView;

class GoogleLoginWindow : public QDialog {
    Q_OBJECT
private:
    static const char* const DEFAULT_URL;

    QStackedLayout* stacked;
    QWidget* loadingIndicatorCtr;
    QWidget* failedLoadingPage;
    QWidget* certErrorPage;
    QWidget* webviewPage;
    QWidget* manualLoginPage;
    QString pAccountIdentifier;
    QString pAccountUserId;
    QString pAccountToken;
    bool certificateError = false;
    bool ignoreCertificateError = false;
#ifndef WITHOUT_WEBENGINE
    QWebEngineView* webView;
#else
    void* webView;
#endif

    void setupWebBrowser();
    void injectWebScripts();

    void onUrlChanged(const QUrl &url);
    void onLoadFinished(bool ok);
    void onCookieAdded(const QNetworkCookie &cookie);

public:
    explicit GoogleLoginWindow(QWidget *parent = nullptr);

    void showWebBrowser();

    QString const& accountIdentifier() const { return pAccountIdentifier; }
    QString const& accountUserId() const { return pAccountUserId; }
    QString const& accountToken() const { return pAccountToken; }

    void setAccountIdentifier(const QString& identifier) { pAccountIdentifier = identifier; }
};

class GoogleLoginWindowApi : public QObject {
    Q_OBJECT

private:
    GoogleLoginWindow* window;

public:
    GoogleLoginWindowApi(GoogleLoginWindow* window, QObject *parent) : QObject(parent), window(window) {}

public slots:
    void showWebBrowser() { window->showWebBrowser(); }
    void setAccountIdentifier(const QString& identifier) { window->setAccountIdentifier(identifier); }

};

#ifndef WITHOUT_WEBENGINE
class WebPage : public QWebEnginePage {
    Q_OBJECT
public:
    explicit WebPage(QWidget * wid) : QWebEnginePage(wid) {}
signals:
    bool verifyCertificateError(QString url, QString errormsg);
protected:
    bool certificateError(const QWebEngineCertificateError& err);
};
#endif

#endif // LOGINWINDOW_H
