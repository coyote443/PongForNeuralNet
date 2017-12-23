#ifndef PONGMODEL_H
#define PONGMODEL_H

#include <QPoint>
#include <QSize>
#include <QString>
#include <QVector>

typedef QVector<int> Row;
typedef QVector<Row> Map;

enum {WIDTH = 90, HEIGHT = 10, PADDLE_SIZE = 5, FIRST_LOC = 0, SECOND_LOC = 1};
enum {GATE = -1, EMPTY = 0, WALL = 1, PADDLE = 2, BALL = 3};

class Ball{
public:
    Ball(QPoint myPos);
    void    setPosition();
    bool    moveMe(Map & map);
    QPoint  myPos();
private:
    struct Reflect{bool x, y;};
    QPoint  m_Position;
    Reflect m_Reflection;

};

class Paddle{
public:
    Paddle(int mySize, QSize mapSize, int givenSpace);
    void    moveMe(bool Up, bool Left);
    QPoint  myPos();
    int     mySize();
private:
    QPoint  m_Position;
    int     m_Size;
};


class Playmap{
public:
    Playmap(QSize mapSize);
    void clearMap();
    void putBall(Ball * ball);
    void putPaddle(Paddle * paddle);
    void drawMap() const;
    Map & myMap();

private:
    void makeMap();
    void makeWallsAndGates();

    QSize   m_MapSize;
    Map     m_Map;
};



class Player{
public:
    Player(QString myName);
    QString toString();
private:
    int         m_Score;
    QString     m_Name;
};

class Gamespace{
public:
    Gamespace();

private:
    QSize   GAME_SIZE;
    QPoint  CENTER_POS;
    Player  * m_FirstPlayer, * m_SecondPlayer;
    Paddle  * m_FirstPaddle, * m_SecondPaddle;
    Ball    * m_Ball;
    Playmap * m_Map;
};


class PongModel
{
public:
    Player * whoWin();
    PongModel();
private:
    Gamespace * m_Gamespace;
};

#endif // PONGMODEL_H
