#ifndef TIMEDIALOG_H
#define TIMEDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QPushButton>

class TimeDialog : public QDialog {
    Q_OBJECT
public:
    explicit TimeDialog(QWidget *parent = nullptr);
    int getTotalSeconds() const;

private:
    QSpinBox *hoursSpin;
    QSpinBox *minutesSpin;

    void setupEditStyle();
};

#endif // TIMEDIALOG_H
