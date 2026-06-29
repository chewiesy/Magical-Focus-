#include "mainwindow.h"
#include "timedialog.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFontDatabase>
#include <QGraphicsOpacityEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowTitle("Knight Focus");
    setFixedSize(400, 420);
    setupLayout();
    setupConnections();
    setupStyle();

    countdown = new QTimer(this);

    connect(countdown, &QTimer::timeout, this, [this]() {
        if (secondsLeft > 0) {
            secondsLeft--;
            updateTimerDisplay();
        } else {
            countdown->stop();
            state = Idle;
        }
    });
    updateTimerDisplay();
}

//                               ~LAYOUT~

void MainWindow::setupLayout() {
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainWindow = new QVBoxLayout(central);
    mainWindow->setSpacing(10);
    mainWindow->setContentsMargins(16, 4, 16, 16);

    //FRAME
    QHBoxLayout *titleBar = new QHBoxLayout();
    titleBar->setContentsMargins(8, 2, 6, 0);

    QLabel *titleLabel = new QLabel("MAGICAL FOCUS <3", this);
    titleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QPushButton *minimizeBtn = new QPushButton(this);
    minimizeBtn->setText("");
    minimizeBtn->setFixedSize(24, 24);
    minimizeBtn->setStyleSheet(
        "QPushButton {"
        "border-image: url(:/buttons/buttons/hide1.png) 0 0 0 0 stretch stretch;"
        "background: transparent;"
        "border: none;"
        "}"
        "QPushButton:pressed {"
        "opacity: 0.7;"
        "}"
    );

    QPushButton *closeBtn = new QPushButton(this);
    closeBtn->setText("");
    closeBtn->setFixedSize(24, 24);
    closeBtn->setStyleSheet(
        "QPushButton {"
        "border-image: url(:/buttons/buttons/close1.png) 0 0 0 0 stretch stretch;"
        "background: transparent;"
        "border: none;"
        "}"
        "QPushButton:pressed {"
        "opacity: 0.7;"
        "}"
    );

    minimizeBtn->setFixedSize(20, 20);
    closeBtn->setFixedSize(20, 20);

    QString titleBtnStyle =
        "QPushButton {"
        "font-family: \"Press Start 2P\";"
        "font-size: 8px;"
        "color: #F5F5F4;"
        "background: transparent;"
        "border: 1px solid #7D80DA;"
        "}"
        "QPushButton:hover {"
        "background-color: #7D80DA;"
        "}";


    titleBar->addWidget(titleLabel);
    titleBar->addStretch();
    titleBar->addWidget(minimizeBtn);
    titleBar->addWidget(closeBtn);

    connect(minimizeBtn, &QPushButton::clicked, this, &QWidget::showMinimized);
    connect(closeBtn, &QPushButton::clicked, this, &QWidget::close);

    //TOP ROW - TIMER
    QHBoxLayout *topRow = new QHBoxLayout();

    resetBtn = new QPushButton(this);
    resetBtn->setText("");
    resetBtn->setFixedSize(35, 35);

    timerLabel = new QLabel("25:00", this);
    timerLabel->setAlignment(Qt::AlignCenter);
    timerLabel->setFixedHeight(70);

    editBtn = new QPushButton(this);
    editBtn->setText("");
    editBtn->setFixedSize(35, 35);

    topRow->addWidget(resetBtn);
    topRow->addWidget(timerLabel);
    topRow->addWidget(editBtn);

    QHBoxLayout *midRow = new QHBoxLayout();

    startBtn = new QPushButton(this);
    startBtn->setFixedSize(80, 30);

    midRow->addWidget(startBtn);

    //CHARCTERS
    charsRow = new QHBoxLayout();
    charsRow->setSpacing(8);

    QString labels[3] = {"KNIGHT", "MONK", "SORCER"};

    mainWindow->addLayout(titleBar);
    mainWindow->addLayout(topRow);
    mainWindow->addSpacing(20);
    mainWindow->addLayout(midRow);
    mainWindow->addLayout(charsRow);
    updateCharUI();
}


//                               ~STYLE~

void MainWindow::setupStyle() {
    centralWidget()->setObjectName("central");
    centralWidget()->setStyleSheet(
        "#central {"
        "border-image: url(:/background/background/cool.png) 0 0 0 0 stretch stretch;"
        "border-radius:12px;"
        "}"
        "QLabel {"
        "color: white;"
        "}"
    );

    int id = QFontDatabase::addApplicationFont(":/fonts/PressStart2P-Regular.ttf");

    qDebug() << "Font id:" << id;
    qDebug() << "Families:" << QFontDatabase::applicationFontFamilies(id);

    if (id == -1) {
        qDebug() << "Font not loaded!";
        return;
    }

    QString fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);

    timerLabel->setStyleSheet(
        "font-family: \"Press Start 2P\";"
        "font-size: 28px;"
        "color: #F5F5F4;"
        "background: transparent;"
        "padding: 8px 16px;"
        );

    QPixmap test(":/buttons/background/btn_a.png");
    qDebug() << "btn_a null?" << test.isNull();

    QString editBtnStyle = R"(
        QPushButton {
        border-image: url(:/buttons/background/sword1.png) 0 0 0 0 stretch stretch;
        background: transparent;
        border: none;
        }
        QPushButton:pressed {
        border-image: url(:/buttons/background/sword2.png) 0 0 0 0 stretch stretch;
        }

    )";

    QString resetBtnStyle = R"(
        QPushButton {
        border-image: url(:/buttons/background/sparks1.png) 0 0 0 0 stretch stretch;
        background: transparent;
        border: none;
        }
        QPushButton:pressed {
        border-image: url(:/buttons/background/sparks2.png) 0 0 0 0 stretch stretch;
        }

    )";

    editBtn->setStyleSheet(editBtnStyle);
    resetBtn->setStyleSheet(resetBtnStyle);

    startBtn->setStyleSheet(
        "QPushButton {"
        "border-image: url(:/buttons/background/act.png) 0 0 0 0 stretch stretch;"
        "background: transparent;"
        "font-family: \"Press Start 2P\";"
        "font-size: 8px;"
        "color: #F5F5F4;"
        "}"
        "QPushButton:pressed {"
        "border-image: url(:/buttons/background/pass.png) 0 0 0 0 stretch stretch;"
        "padding: 2px;"
        "}"
        );

}

//                           ~WINDOW LOGIC~
void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - dragPosition);
    }

}

//                           ~CHARACTERS' LOGIC~

void MainWindow::updateCharUI(){
    int count = 3;

    int left = (activeChar - 1 + count) % count;
    int center = activeChar;
    int right = (activeChar + 1) % count;

    QLayoutItem *item;
    while((item = charsRow->takeAt(0)) != nullptr) {
        if (item -> widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    QString sprites[3] = {
        ":/sprites/sprites/knight.PNG",
        ":/sprites/sprites/monk.PNG",
        ":/sprites/sprites/witch.PNG"
    };

    charclick *leftCard = new charclick(this);
    QPixmap leftPixmap(sprites[left]);
    qDebug() << "Left pixmap null?" << leftPixmap.isNull();
    leftCard->setPixmap(leftPixmap.scaled(90, 180, Qt::KeepAspectRatio, Qt::FastTransformation));
    QGraphicsOpacityEffect *leftEffect = new QGraphicsOpacityEffect();
    leftEffect->setOpacity(0.4);
    leftCard->setGraphicsEffect(leftEffect);


    charclick *centerCard = new charclick(this);
    QPixmap centerPixmap(sprites[center]);
    centerCard->setPixmap(centerPixmap.scaled(150, 250, Qt::KeepAspectRatio, Qt::FastTransformation));


    charclick *rightCard = new charclick(this);
    QPixmap rightPixmap(sprites[right]);
    rightCard->setPixmap(rightPixmap.scaled(90, 180, Qt::KeepAspectRatio, Qt::FastTransformation));
    QGraphicsOpacityEffect *rightEffect = new QGraphicsOpacityEffect();
    rightEffect->setOpacity(0.4);
    rightCard->setGraphicsEffect(rightEffect);

    charsRow->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    QFont pixelFont("Press Start 2P", 6);

    QWidget *leftContainer = new QWidget(this);
    leftContainer->setFixedHeight(200);
    leftContainer->setMinimumWidth(100);
    QVBoxLayout *leftLay = new QVBoxLayout(leftContainer);
    leftLay->setSpacing(12);
    leftLay->setContentsMargins(0,0,0,0);
    leftLay->addWidget(leftCard);
    leftLay->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    QLabel *leftName = new QLabel(charNames[left], this);
    leftName->setAlignment(Qt::AlignHCenter);
    leftName->setFont(pixelFont);
    leftName->setStyleSheet(
        "color: #BBA0A0;"
        );
    leftLay->addWidget(leftName);

    QWidget *centerContainer = new QWidget(this);
    centerContainer->setFixedHeight(200);
    QVBoxLayout *centerLay = new QVBoxLayout(centerContainer);
    centerLay->setSpacing(12);
    centerLay->setContentsMargins(0,0,0,0);
    centerLay->addWidget(centerCard);
    centerLay->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    QLabel *centerName = new QLabel(charNames[center], this);
    centerName->setAlignment(Qt::AlignHCenter);
    centerName->setFont(pixelFont);
    centerName->setStyleSheet(
        "color: #F5F5F4;"
        );
    centerLay->addWidget(centerName);

    QWidget *rightContainer = new QWidget(this);
    rightContainer->setFixedHeight(200);
    QVBoxLayout *rightLay = new QVBoxLayout(rightContainer);
    rightLay->setSpacing(12);
    rightLay->setContentsMargins(0,0,0,0);
    rightLay->addWidget(rightCard);
    rightLay->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    QLabel *rightName = new QLabel(charNames[right], this);
    rightName->setAlignment(Qt::AlignHCenter);
    rightName->setFont(pixelFont);
    rightName->setStyleSheet(
        "color: #BBA0A0;"
        );
    rightLay->addWidget(rightName);

    charsRow->addWidget(leftContainer);
    charsRow->addWidget(centerContainer);
    charsRow->addWidget(rightContainer);

    connect(leftCard, &charclick::clicked, this, &MainWindow::switchChars);
    connect(rightCard, &charclick::clicked, this, &MainWindow::switchChars);

}

void MainWindow::switchChars() {
    activeChar = (activeChar + 1) % 3;
    updateCharUI();
    resetTimer();

    player->setSource(QUrl(musicTracks[activeChar]));
}

//                          ~TIMER'S LOGIC~

void MainWindow::setupConnections() {
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);
    player->setLoops(QMediaPlayer::Infinite);

    connect(startBtn, &QPushButton::clicked, this, &MainWindow::startTimer);

    connect(editBtn, &QPushButton::clicked, this, &MainWindow::editTimer);

    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::resetTimer);
}

void MainWindow::startTimer() {
    qDebug() << "Starttimer called! State:" << state;
    if (state == Idle || state == Paused) {
        countdown->start(1000);
        state = Running;

        qDebug() << "Music track:" << musicTracks[activeChar];
        player->setSource(QUrl(musicTracks[activeChar]));
        qDebug() << "Player state before play:" << player->playbackState();
        player->play();
        qDebug() << "Player state after play:" << player->playbackState();
        qDebug() << "Playerr error:" << player->errorString();

        // startBtn->setText("PAUSE");
    } else if (state == Running) {
        countdown->stop();
        state = Paused;
        player->pause();

        // startBtn->setText("FOCUS");
    }

}

void MainWindow::resetTimer() {
    countdown->stop();
    state = Idle;
    secondsLeft = defaultSeconds;
    updateTimerDisplay();
    player->stop();
}

void MainWindow::editTimer() {
    if (state == Running) return;

    TimeDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        secondsLeft = dialog.getTotalSeconds();
        defaultSeconds = secondsLeft;
        updateTimerDisplay();
    }
}

void MainWindow::updateTimerDisplay() {
    int hours = secondsLeft / 3600;
    int minutes = (secondsLeft % 3600) / 60;
    int seconds = secondsLeft % 60;

    QString text;

    if (hours > 0) {
        text = QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));

    } else {
        text = QString("%1:%2")
            .arg(minutes, 2, 10, QChar('0'))
                   .arg(seconds, 2, 10, QChar('0'));
    }

    timerLabel->setText(text);

    timerLabel->setText(text);
}

MainWindow::~MainWindow() = default;
