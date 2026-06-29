#include "charclick.h"

charclick::charclick(QWidget *parent)
    : QLabel(parent)
{}

void charclick::mousePressEvent(QMouseEvent *event) {
    emit clicked();
    QLabel::mousePressEvent(event);
}