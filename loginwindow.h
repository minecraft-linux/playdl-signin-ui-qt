#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

class QStackedWidget;
class QWebEngineView;

class LoginWindow : public QMainWindow {
    Q_OBJECT
private:
    static const char* const DEFAULT_URL;

    QStackedWidget* stacked;
    QWidget* loadingIndicatorCtr;
    QWebEngineView* webView;

    void setupWebBrowser();
    void injectWebScripts();

public:
    explicit LoginWindow(QWidget *parent = nullptr);

    void showWebBrowser();
};

class LoginWindowApi : public QObject {
    Q_OBJECT

private:
    LoginWindow* window;

public:
    LoginWindowApi(LoginWindow* window, QObject *parent) : QObject(parent), window(window) {}

public slots:
    void showWebBrowser() { window->showWebBrowser(); }

};

#endif // LOGINWINDOW_H
