#ifndef CHARCLICK_H
#define CHARCLICK_H

#include <QLabel>

class charclick : public QLabel {
    Q_OBJECT
public:
    explicit charclick(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void clicked();
};

#endif // CHARCLICK_H
