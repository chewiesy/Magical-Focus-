#include "timedialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFontDatabase>

TimeDialog::TimeDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Set Focus Time");
    setFixedSize(250, 150);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QHBoxLayout *timeLayout = new QHBoxLayout();

    hoursSpin = new QSpinBox(this);
    hoursSpin->setRange(0, 12);
    hoursSpin->setSuffix(" h");

    minutesSpin = new QSpinBox(this);
    minutesSpin->setRange(0, 59);
    minutesSpin->setSuffix(" m");

    timeLayout->addWidget(hoursSpin);
    timeLayout->addWidget(minutesSpin);

    QPushButton *okBtn = new QPushButton("OK", this);
    QPushButton *cancelBtn = new QPushButton("Cancel", this);

    connect(okBtn, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    mainLayout->addLayout(timeLayout);
    mainLayout->addLayout(btnLayout);

    setupEditStyle();
}

int TimeDialog::getTotalSeconds() const {
    return hoursSpin->value() * 3600 + minutesSpin->value() * 60;
}

void TimeDialog::setupEditStyle() {
    int id = QFontDatabase::addApplicationFont(":/fonts/PressStart2P-Regular.ttf");

    qDebug() << "Font id:" << id;
    qDebug() << "Families:" << QFontDatabase::applicationFontFamilies(id);

    if (id == -1) {
        qDebug() << "Font not loaded!";
        return;
    }

    QString fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);

    this->setStyleSheet(
        "Qdialog {"
        "background-color: #2D1B69;"
        "}"
        "QLabel {"
        "font-family: \"Press Start 2P\";"
        "font-size: 8px;"
        "color: #F5F5F4;"
        "background-color: #55418B;"
        "border: 2px solid #7D80DA;"
        "border-radius: 8px;"
        "padding: 4px;"
        "}"
        "QPushButton {"
        "font-family: \"Press Start 2P\";"
        "font-size: 8px;"
        "color: #F5F5F4;"
        "background-color: #C578EE;"
        "border: 2px solid #AB43D3;"
        "border-radius: 8px;"
        "padding: 4px 8px;"
        "}"

        "QPushButton:pressed {"
        "background-color: #7D80DA;"
        "}"
    );
}