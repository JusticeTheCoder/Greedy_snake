#include "mainwindow.h"
#include <QLayout>
#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QRectF>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /*data*/
    life = 3;
    score = 0;
    gamestate = false;
    snake.append(Index(15,15));
    fruit.append(Index(5,5));
    fruit.append(Index(25,25));
    timer = new QTimer;
    fruitTimer = new QTimer;
    dir = u;
    /*subwindow1*/
    subwindow1 = new QWidget();
    subwindow1 -> resize(800, 600);
    setWindowTitle("贪吃蛇ver0.1");
    setFixedSize(800,600);
    QPixmap coverpixmp("D:\\img\\cover.png");
    coverpic = new QLabel();
    coverpixmp = coverpixmp.scaled(800, 400);
    coverpic -> setPixmap(coverpixmp);
    coverpic -> resize(800,400);
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QVBoxLayout *mainLayout = new QVBoxLayout(subwindow1);
    startBtn = new QPushButton();
    quitBtn = new QPushButton();
    startBtn -> setText("开始游戏");
    quitBtn -> setText("退出游戏");
    btnLayout -> addWidget(startBtn);
    btnLayout -> addSpacing(int(0.3*subwindow1 -> width()));
    btnLayout -> addWidget(quitBtn);
    mainLayout -> setMargin(20);
    mainLayout -> addWidget(coverpic);
    mainLayout -> addLayout(btnLayout);
    /*subwindow2*/
    subwindow2 = new QWidget();
    subwindow2 -> resize(800, 600);
    QVBoxLayout *mainLayout2 = new QVBoxLayout(subwindow2);
    for(int i = 0; i < 3; i++)
    {
        save[i] = new QPushButton();
        QString temp;
        temp.setNum(i);
        save[i] -> setText("存档"+temp);
        save[i] -> setFont(QFont("宋体",36));
        mainLayout2 -> addWidget(save[i]);
    }
    backBtn = new QPushButton();
    backBtn -> setText("返回");
    //backBtn -> resize(300, 100);
    mainLayout2 -> addWidget(backBtn);
    /*stack*/
    stack = new QStackedWidget(this);
    stack -> resize(800, 600);
    stack -> addWidget(subwindow1);
    /*GameWindow*/
    gamewindow = new QWidget();
    gamewindow -> setFixedSize(800,600);
    gamewindow -> setFont(QFont("宋体",16));
    gamewindow -> setWindowTitle("贪吃蛇：游戏开始！");
    gamewindow -> installEventFilter(this);
    subwindow3 = new QWidget(gamewindow);
    subwindow3 -> setGeometry(0,0,600,600);
    subwindow3 -> setStyleSheet("background-color:grey;");
    subwindow3 -> installEventFilter(this);
    subwindow4 = new QWidget(gamewindow);
    subwindow4 -> setGeometry(600,0,200,600);
    rstBtn = new QPushButton();
    exitBtn = new QPushButton();
    rstBtn -> setText("开始游戏");
    exitBtn -> setText("结束游戏");
    score1 = new QLabel();
    score1 -> setText("分数：");
    score2 = new QLabel();
    score2 -> setText("0");
    life1 = new QLabel();
    life1 -> setText("生命：");
    life2 = new QLabel();
    life2 -> setText("***");
    QGridLayout *dataLayout = new QGridLayout(subwindow4);
    selectsnake = new QComboBox();
    selectsnake -> addItem("基础蛇");
    selectsnake -> addItem("再生蛇");
    selectsnake -> addItem("穿梭蛇");
    selectsnake -> addItem("缓慢蛇");
    QVariant v(0);
    for(int i = 1; i < 4; i++)
    {
        selectsnake->setItemData(i, v, Qt::UserRole - 1);
    }
    dataLayout -> addWidget(score1,0,0);
    dataLayout -> addWidget(score2,0,1);
    dataLayout -> addWidget(life1,1,0);
    dataLayout -> addWidget(life2,1,1);
    dataLayout -> addWidget(selectsnake, 2, 1);
    dataLayout -> addWidget(rstBtn,3,1);
    dataLayout -> addWidget(exitBtn,4,1);
    /*slots*/
    stack -> addWidget(subwindow2);
    connect(quitBtn, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(timer, SIGNAL(timeout()), this, SLOT(timeOut()));
    connect(startBtn, SIGNAL(clicked()), this, SLOT(startClick()));
    connect(backBtn, SIGNAL(clicked()), this, SLOT(startClick()));
    connect(rstBtn, SIGNAL(clicked()), this, SLOT(gameStart()));
    for(int i = 0 ; i < 3; i++)
    {
        connect(save[i], SIGNAL(clicked()), this ,SLOT(saveSelect()));
    }
    connect(exitBtn, SIGNAL(clicked()), this, SLOT(exitclicked()));
    connect(fruitTimer, SIGNAL(timeout()), this ,SLOT(fruitTimeOut()));
}
MainWindow::~MainWindow()
{    
}

void MainWindow::startClick()
{
    stack ->setCurrentIndex((stack->currentIndex()+1)%(stack->count()));
}

void MainWindow::saveSelect()
{
    int senderNo = 0;
    for(int i = 0 ; i< 3; i++)
    {
        if(sender() == save[i])
        {
            senderNo = i;
            break;
        }
    }
    QString filename;
    filename.setNum(senderNo);
    filename = "save"+filename+".txt";

    QDir dir;
    dir = dir.home();
    dir.cd("Documents/My Games");
    if(dir.exists("Greed Snake") == false)
    {
        dir.mkdir("Greed Snake");
    }
    dir.cd("Greed Snake");
    QFile save(dir.absolutePath()+"/"+filename);
    if(dir.exists(filename) == false)
    {
        save.open( QIODevice::ReadWrite | QIODevice::Text );
        save.close();
        save.open( QIODevice::ReadWrite | QIODevice::Text );
        QTextStream out(&save);
        out << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
        save.close();
    }
    savedt = new SaveData(senderNo, &save);
    unlock();
    savedt ->showInfo();
    this -> hide();
    gamewindow -> show();
}

void MainWindow::exitclicked()
{
    QDir dir;
    dir = dir.home();
    dir.cd("Documents/My Games/Greed Snake");
    QString filename;
    filename.setNum(savedt->no);
    filename = "save"+filename+".txt";
    QFile save(dir.absolutePath()+"/"+filename);
    savedt ->writeback(&save);
    gamewindow -> hide();
    stack -> setCurrentIndex(0);
    this -> show();
}

void MainWindow::keyPress(QKeyEvent *event)
{
    if(gamestate == false)
        return;
    else
    {

        switch (event ->key())
        {
        case Qt::Key_W:
        {
            if (dir != d)
                dir = u;
            break;
        }
        case Qt::Key_A:
        {
            if (dir != r)
                dir = l;
            break;
        }
        case Qt::Key_S:
        {
            if (dir != u)
                dir = d;
            break;
        }
        case Qt::Key_D:
        {
            if (dir != l)
                dir = r;
            break;
        }
        case Qt::Key_R:
        {
            if (selectsnake->currentIndex()==3)
            {
                for(int k = 0; k < (snake.size()/2); k++)
                    snake.swapItemsAt(k,snake.size()-(1+k));
                if(dir == u) dir = d;
                else if(dir == l) dir = r;
                else if(dir == r) dir = l;
                else if(dir == d) dir =u;
            }
            break;
        }
        default:
            break;
        }
    }
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == subwindow3 && event->type() == QEvent::Paint)
    {
        qDebug() << "ok";
        QPainter painter(subwindow3);
        QPen pen;
        QBrush brush;
        QFont font("宋体", 12, QFont::ExtraLight,false);
        painter.setRenderHint(QPainter::Antialiasing);
        pen.setColor(Qt::black);
        switch (selectsnake->currentIndex())
        {
        case 0:brush.setColor(QColor(124,205,124));break;
        case 1:brush.setColor(QColor(205,85,85));break;
        case 2:brush.setColor(QColor(72,61,139));break;
        case 3:brush.setColor(QColor(139,115,85));break;
        }
        brush.setStyle(Qt::SolidPattern);
        painter.setPen(pen);
        painter.setBrush(brush);
        for(int i=0; i<snake.length(); i++)
        {
            painter.drawRect(QRect(snake.at(i).x*20,snake.at(i).y*20,20,20));
        }
        brush.setColor(Qt::red);
        painter.setBrush(brush);
        for(int i=0; i<fruit.length(); i++)
        {
            painter.drawEllipse(QRect(fruit.at(i).x*20,fruit.at(i).y*20,20,20));
        }

        brush.setColor(Qt::black);
        painter.setBrush(brush);
        for(int i = 0; i < walls.size(); i++)
        {
            for(int j = 0; j < walls[i].size(); j++)
            {
                if(walls[i][j] == 1)
                    painter.drawRect(QRect(j*20,i*20,20,20));
            }
        }

    }
    if(watched == gamewindow && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        keyPress(keyEvent);
    }
    return QMainWindow::eventFilter(watched,event);     //将事件传递给父类
}

void MainWindow::timeOut()
{
    if(dir==l)
    {
        if(snake.front().x > 0)
        {
            snake.insert(0, Index(snake.front().x-1, snake.front().y));
        }
        else if(selectsnake->currentIndex() == 2)
        {
            snake.insert(0, Index(29, snake.front().y));
        }
        else
        {
            gameOver();
        }
    }
    else if(dir == u)
    {
        if(snake.front().y > 0)
        {
            snake.insert(0, Index(snake.front().x, snake.front().y-1));
        }
        else if(selectsnake->currentIndex() == 2)
        {
            snake.insert(0, Index(snake.front().x, 29));
        }
        else
        {
            gameOver();
        }

    }
    else if(dir == r)
    {
        if(snake.front().x < 29)
        {
            snake.insert(0, Index(snake.front().x+1, snake.front().y));
        }
        else if(selectsnake->currentIndex() == 2)
        {
            snake.insert(0, Index(0, snake.front().y));
        }
        else
        {
            gameOver();
        }

    }
    else if(dir == d)
    {
        if(snake.front().y < 29)
        {
            snake.insert(0, Index(snake.front().x, snake.front().y+1));
        }
        else if(selectsnake->currentIndex() == 2)
        {
            snake.insert(0, Index(snake.front().x, 0));
        }
        else
        {
            gameOver();
        }

    }
    bool eat = false;
    for(auto i = fruit.begin(); i != fruit.end(); i++)
    {
        if(i->x == snake.front().x && i->y == snake.front().y)
        {
            fruit.erase(i);
            eat = true;
            score += 5;
            scorechanged();
            break;
        }
    }
    for(auto i = snake.begin()+1; i != snake.end(); i++)
    {
        if(i->x == snake.front().x && i->y == snake.front().y)
        {
            gameOver();
            break;
        }
    }
    if(walls[snake.front().y][snake.front().x] == 1)
    {
        gameOver();
    }
    if(eat == false)
        snake.pop_back();
    gamewindow -> update();
}

void MainWindow::fruitTimeOut()
{
    QTime time;
    time= QTime::currentTime();
    qsrand(uint(time.msec()+time.second()*1000));
    if(fruit.length() < 5)
    {
        int x = qrand() % 30;
        int y = qrand() % 30;
        while(walls[y][x]==1)
        {
            x = qrand() % 30;
            y = qrand() % 30;
        }
        fruit.append(Index(x,y));
    }
}
void MainWindow::gameStart()
{
    selectsnake -> setEnabled(false);
    QTime time;
    time= QTime::currentTime();
    qsrand(uint(time.msec()+time.second()*1000));
    int bond = score / 10;
    if (bond < 5) bond = 5;
    if (bond > 30) bond = 30;
    walls.clear();
    for(int i = 0; i< 30; i++)
    {
        walls.push_back({});
        for(int j = 0; j<30; j++)
        {
            if(qrand()%200>bond)
                walls[i].push_back(0);
            else
                walls[i].push_back(1);
        }
    }
    walls[15][15] = 0;
    walls[14][15] = 0;
    snake.clear();
    snake.append(Index(15,15));
    gamestate = true;
    int t = selectsnake -> currentIndex() == 3? 200:100;
    timer ->start(t);
    fruitTimer -> start(5000);
}

void MainWindow::gameOver()
{
    fruitTimer ->stop();
    timer -> stop();
    gamestate = false;
    life--;
    lifechanged();
    if(life == 0)
    {
        gameDead();
    }

}

void MainWindow::gameDead()
{
    selectsnake -> setEnabled(true);
    if(score > savedt->highScore)
    {
        savedt -> highScore = score;
    }
    score = 0;
    scorechanged();
    life = 3;
}

void MainWindow::lifechanged()
{
    QString temp("");
    for(int i = 0 ; i < life ; i++)
        temp += "*";
    life2 -> setText(temp);
}

void MainWindow::scorechanged()
{
    QString temp;
    score2 -> setText(temp.setNum(score));
    if(score % 25 == 0 && selectsnake->currentIndex() == 1)
    {
        life ++;
        lifechanged();
    }
    if(score % 100 == 0)
    {
        savedt -> unlocked[score/100] = 1;
        unlock();
    }

}

void MainWindow::unlock()
{
    for(int i = 0; i< 3; i++)
    {
        if(savedt->unlocked[i]==1)
            selectsnake->setItemData(i+1, QVariant(-1), Qt::UserRole - 1);
        else
            selectsnake->setItemData(i+1, QVariant(0), Qt::UserRole - 1);
    }
}


