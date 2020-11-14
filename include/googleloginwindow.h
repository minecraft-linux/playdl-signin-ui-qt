#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QWebEnginePage>
#include <QWebEngineCertificateError>

class QStackedLayout;
class QNetworkCookie;
class QWebEngineView;

class GoogleLoginWindow : public QDialog {
    Q_OBJECT
private:
    static const char* const DEFAULT_URL;

    QStackedLayout* stacked;
    QWidget* loadingIndicatorCtr;
    QWebEngineView* webView;
    QWidget* failedLoadingPage;
    QWidget* certErrorPage;
    QString pAccountIdentifier;
    QString pAccountUserId;
    QString pAccountToken;
    bool certificateError = false;
    bool ignoreCertificateError = false;

    void setupWebBrowser();
    void injectWebScripts();

public:
    explicit GoogleLoginWindow(QWidget *parent = nullptr);

    void showWebBrowser();

    QString const& accountIdentifier() const { return pAccountIdentifier; }
    QString const& accountUserId() const { return pAccountUserId; }
    QString const& accountToken() const { return pAccountToken; }

    void setAccountIdentifier(const QString& identifier) { pAccountIdentifier = identifier; }

    void onUrlChanged(const QUrl &url);
    void onLoadFinished(bool ok);
    void onCookieAdded(const QNetworkCookie &cookie);
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

class WebPage : public QWebEnginePage {
    Q_OBJECT
public:
    explicit WebPage(QWidget * wid) : QWebEnginePage(wid) {}
signals:
    bool verifyCertificateError(QString url, QString errormsg);
protected:
    bool certificateError(const QWebEngineCertificateError& err);
};

#endif // LOGINWINDOW_H
