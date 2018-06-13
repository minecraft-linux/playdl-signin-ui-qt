#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>

class LoginWindow : public QMainWindow
{
    Q_OBJECT
private:
    static const char* const DEFAULT_URL;

public:
    explicit LoginWindow(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // LOGINWINDOW_H
