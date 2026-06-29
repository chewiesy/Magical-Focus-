#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"charclick.h"
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QHBoxLayout>
#include "QFont"
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPropertyAnimation>
#include <QEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    //TIMER
    enum TimerState {
        Idle,
        Running,
        Paused
    };

    QLabel *timerLabel;
    QPushButton *resetBtn;
    QPushButton *editBtn;
    QPushButton *startBtn;
    QTimer *countdown;
    TimerState state = Idle;
    int secondsLeft = 25 * 60;
    int defaultSeconds = 25 * 60;

    //CHARACTERS
    int activeChar = 0;

    QString charNames[3] = {"KNIGHT", "MONK", "WITCH"};
    charclick *leftChar;
    charclick *midChar;
    charclick *rightChar;

    QHBoxLayout *charsRow;

    //UI SETUP
    void setupLayout();
    void setupConnections();
    void setupStyle();

    //LOGIC
    void updateTimerDisplay();
    void switchChars();
    void updateCharUI();

    //WINDOW LOGIC
    QPoint dragPosition;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    //MUSIC
    QMediaPlayer *player;
    QAudioOutput *audioOutput;

    QString musicTracks[3] = {
        "qrc:/music/music/fave1.mp3",
        "qrc:/music/music/fave2.mp3",
        "qrc:/music/music/fave3.mp3"
    };

private slots:
    //UI REACTIONS
    void startTimer();
    void editTimer();
    void resetTimer();

};
#endif // MAINWINDOW_H
