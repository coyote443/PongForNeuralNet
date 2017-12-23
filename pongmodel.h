#ifndef PONGMODEL_H
#define PONGMODEL_H

#include <QPoint>
#include <QSize>
#include <QString>
#include <QVector>
#include <QObject>

typedef QVector<int> Row;
typedef QVector<Row> Map;

enum {PADDLE_SIZE = 5, FIRST_LOC = 0, SECOND_LOC = 1, FIRST_P = 0, SECOND_P = 1};
enum {FIRST_GATE  = -2, SECOND_GATE = -1, EMPTY = 0, WALL = 1, PADDLE = 2, BALL = 3, UP = 1, DOWN = 0};
enum {POINTS_TO_WIN = 10};



class Ball : QObject{
    Q_OBJECT
public:
    Ball(QPoint myPos);
    void    setPosition(QPoint newPos);
    bool    moveMe(const Map &map);
    QPoint  myPos();

signals:
    void firstGetPoints();
    void secondGetPoints();

private:
    struct Reflect{bool x, y;};
    QPoint  m_Position;
    Reflect m_Reflection;
};



class Paddle{
public:
    Paddle(int mySize, QSize mapSize, int givenSpace);
    void    moveMe(bool upTrueDownFalse);
    QPoint  myPos();
    int     mySize();

private:
    QPoint  m_Position;
    int     m_Size;
    QSize   m_MapSize;
};



class Playmap{
public:
    Playmap(QSize mapSize);
    void clearMap();
    void putBall(Ball * ball);
    void putPaddle(Paddle * paddle);
    void drawMap() const;
    const Map & showMap() const;
    void makeNoise(int wallsNumber);

private:
    void makeMap();
    void makeWallsAndGates();

    QSize   m_MapSize;
    Map     m_Map;
};



class Player : QObject{
    Q_OBJECT
public:
    Player(int myNr);
    void    addPoint();
    void    resetPoints();
    int     showPoints();
    QString showName();

private:
    int     m_Score;
    int     m_PlayerNumber;
    QString m_Name;
};



class PongModel : QObject{
    Q_OBJECT
public:
    PongModel(const int WIDTH, const int HEIGHT);
    PongModel(PongModel &) = delete;
    PongModel & operator=(const PongModel &) = delete;
    const   Map & showMap();
    void    makeNoise(int wallsNumber);
    void    movePaddle(int player, int way);
    void    moveBall();
    void    refresh();
    void    setBasicPositions();
    void    setBasicConfiguration();
    int     showScore(int player);
    QString showName(int player);
    ~PongModel();

public slots:
    void firstPlayerAddPoints();
    void secondPlayerAddPoints();

private:
    const QSize   GAME_SIZE;
    const QPoint  CENTER_POS;
    Player  * m_FirstPlayer, * m_SecondPlayer;
    Paddle  * m_FirstPaddle, * m_SecondPaddle;
    Ball    * m_Ball;
    Playmap * m_PlayMap;
};

#endif // PONGMODEL_H
