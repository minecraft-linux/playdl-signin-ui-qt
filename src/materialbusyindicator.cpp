#include "materialbusyindicator.h"

#include <QtGlobal>
#include <QPainter>

MaterialBusyIndicator::MaterialBusyIndicator(QWidget *parent) : QWidget(parent) {
    pen.setColor(QColor(0, 0xE6, 0x76));
    pen.setWidth(4);

    lastDraw = std::chrono::system_clock::now();

    connect(&timer, &QTimer::timeout, this, &MaterialBusyIndicator::requestRepaint);
    timer.start(1000 / 60.0);

    repaint();
}

void MaterialBusyIndicator::paintEvent(QPaintEvent *event) {
    auto now = std::chrono::system_clock::now();
    float delta = std::chrono::duration_cast<std::chrono::duration<float>>(now - lastDraw).count();
    lastDraw = now;

    rot += delta * rotationSpeed;
    float easedSize = 0.f;
    float off = 0;
    if (sizeCollapse) {
        size = qMax<float>(size - delta * sizeCollapseSpeed, 0.f);
        easedSize = size * size;
        if (size <= 0) {
            size = 0;
            sizeCollapse = false;
            rot += maxSize - minSize;
        } else {
            off = (maxSize - minSize) * (1 - easedSize);
        }
    } else {
        size = size + delta * sizeSpeed;
        easedSize = (1 - qCos(M_PI * size)) / 2;
        if (size >= 1) {
            size = 1;
            sizeCollapse = true;
        }
    }


    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(pen);
    QRect rect (0, 0, width(), height());
    if (rect.width() > rect.height()) {
        rect.setX(rect.width() / 2 - rect.height() / 2);
        rect.setWidth(rect.height());
    } else if (rect.width() < rect.height()) {
        rect.setY(rect.height() / 2 - rect.width() / 2);
        rect.setHeight(rect.width());
    }
    rect.adjust(pen.width() / 2, pen.width() / 2, -pen.width() / 2, -pen.width() / 2);
    painter.drawArc(rect, (int) ((rot + off) / M_PI * 180 * 16),  (int) ((minSize + (maxSize - minSize) * easedSize) / M_PI * 180 * 16));
}

void MaterialBusyIndicator::requestRepaint() {
    repaint();
}

