#ifndef MATERIALBUSYINDICATOR_H
#define MATERIALBUSYINDICATOR_H

#include <chrono>
#include <QWidget>
#include <QTimer>
#include <QPen>
#include <QtMath>

class MaterialBusyIndicator : public QWidget
{
    Q_OBJECT
protected:
    QTimer timer;
    std::chrono::system_clock::time_point lastDraw;

    QPen pen;

    float rotationSpeed = 3.f;
    float sizeSpeed = 2.f;
    float sizeCollapseSpeed = 1.f;
    float minSize = 0.1 * M_PI;
    float maxSize = 1.5 * M_PI;

    float rot = M_PI * 3 / 2;
    float size = 0.f;
    bool sizeCollapse = false;

public:
    explicit MaterialBusyIndicator(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

protected slots:
    void requestRepaint();
};

#endif // MATERIALBUSYINDICATOR_H
