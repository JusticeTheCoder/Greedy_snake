#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLayout>
#include <QStackedWidget>
#include "savedata.h"
#include <QTimer>
#include <QComboBox>
#include <QList>
#include <QVector>
#include <QWidget>
#include <QKeyEvent>
#include <QTime>

struct Index
{
    int x;
    int y;
    Index(int a, int b) : x(a), y(b)
    {

    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool eventFilter(QObject *watched, QEvent *event);
    void keyPress(QKeyEvent *event);
private:
    void gameOver();
    void gameDead();
    void lifechanged();
    void scorechanged();
    void unlock();
    QWidget *subwindow1;
    QWidget *subwindow2;
    QWidget *gamewindow;
    QWidget *subwindow3;
    QWidget *subwindow4;
    QTimer *timer;
    QTimer *fruitTimer;
    QLabel *coverpic;
    QLabel *score1;
    QLabel *score2;
    QLabel *life1;
    QLabel *life2;
    QPushButton *startBtn;
    QPushButton *quitBtn;
    QPushButton *save[3];
    QPushButton *backBtn;
    QPushButton *rstBtn;
    QPushButton *exitBtn;
    QComboBox *selectsnake;
    QStackedWidget *stack;
    SaveData *savedt;
    enum directions{l,u,r,d} dir;
    QList<Index> snake;
    QVector<QVector<int>> walls;
    QList<Index> fruit;
    int life;
    int score;
    bool gamestate;
private slots:
    void startClick();
    void saveSelect();
    void exitclicked();
    void timeOut();
    void fruitTimeOut();
    void gameStart();
};
#endif // MAINWINDOW_H
