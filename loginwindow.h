#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

class QStackedWidget;
class QNetworkCookie;
class QWebEngineView;

class LoginWindow : public QMainWindow {
    Q_OBJECT
private:
    static const char* const DEFAULT_URL;

    QStackedWidget* stacked;
    QWidget* loadingIndicatorCtr;
    QWebEngineView* webView;
    QString accountIdentifier;
    QString accountUserId;
    QString accountToken;

    void setupWebBrowser();
    void injectWebScripts();

public:
    explicit LoginWindow(QWidget *parent = nullptr);

    void showWebBrowser();
    void setAccountIdentifier(const QString& identifier) { accountIdentifier = identifier; }

    void onUrlChanged(const QUrl &url);
    void onCookieAdded(const QNetworkCookie &cookie);
};

class LoginWindowApi : public QObject {
    Q_OBJECT

private:
    LoginWindow* window;

public:
    LoginWindowApi(LoginWindow* window, QObject *parent) : QObject(parent), window(window) {}

public slots:
    void showWebBrowser() { window->showWebBrowser(); }
    void setAccountIdentifier(const QString& identifier) { window->setAccountIdentifier(identifier); }

};

#endif // LOGINWINDOW_H
