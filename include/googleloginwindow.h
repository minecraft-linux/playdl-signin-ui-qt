#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

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
    QString pAccountIdentifier;
    QString pAccountUserId;
    QString pAccountToken;

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

#endif // LOGINWINDOW_H
