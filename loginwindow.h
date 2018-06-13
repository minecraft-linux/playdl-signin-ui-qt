#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

class QStackedWidget;
class QWebEngineView;

class LoginWindow : public QMainWindow
{
    Q_OBJECT
private:
    static const char* const DEFAULT_URL;

    QStackedWidget* stacked;
    QWidget* loadingIndicatorCtr;
    QWebEngineView* webView;

public:
    explicit LoginWindow(QWidget *parent = nullptr);

protected slots:
    void onPageLoaded(bool ok);
};

#endif // LOGINWINDOW_H
