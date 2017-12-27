#ifndef PONGMODEL_H
#define PONGMODEL_H

#include <QPoint>
#include <QSize>
#include <QString>
#include <QVector>
#include <QObject>

typedef QVector<int> Row;
typedef QVector<Row> Map;

enum {POINTS_TO_WIN = 10};

enum {FIRST_GATE  = -2, SECOND_GATE = -1, EMPTY = 0, CENTLINE = 1, BALL = 2, WALL = 3, PAD_CENTER = 4, PAD_DEV = 5, PAD_BORDER = 6};
enum {PAD_SIZE = 5, FIRST_LOC = 0, SECOND_LOC = 1, FIRST_PLAYER = 0, SECOND_PLAYER = 1, UP = 1, DOWN = 0};

class Ball : QObject{
    Q_OBJECT
public:
    Ball(QPoint myPosition);
    void            setPosition(QPoint newPos);
    void            moveMe(const Map &map);
    QPoint          showPos();
    int             showSpeed();

signals:
    void            firstGetPoints();
    void            secondGetPoints();

private:
    void            resetSpeed();
    struct          Reflect{bool x, y;};
    QPoint          m_Position;
    Reflect         m_Reflection;
    int             m_Speed;
    int             m_LastHit;
    bool            m_DrunkBallWayFlag  = false;
    bool            m_isBallDrunk       = false;

    void doBallCollisions(bool &modifier, int nextPos);
    void doBallMovement(QPoint &newPos);
    void doBallPadCenterReflection(QPoint &newPos);
    void doBallDrunkReflection(QPoint &newPos, int &nextPos, QPoint &newFirst, QPoint &newSecond, const Map &map);
    void doBallStdReflections(const Map &map, QPoint &newFirst, int &nextPos, QPoint &newPos, QPoint &newSecond);
};



class Paddle{
public:
    Paddle(int mySize, QSize mapSize, int givenSpace);
    void            moveMe(bool upTrueDownFalse);
    QPoint          myPos();
    int             mySize();
    const QVector<int> & myStructure();

private:
    void            createPadStructure();
    QPoint          m_Position;
    int             m_PadSize;
    QSize           m_MapSize;
    QVector<int>    m_PadStructure;
};



class Playmap{
public:
    Playmap(QSize mapSize);
    void            clearMap();
    void            putBall(Ball * ball);
    void            putPaddle(Paddle * paddle);
    void            drawMap() const;
    const Map &     showMap() const;
    void            makeNoise(int wallsNumber);

    void makeCenterLine();

private:
    void            makeEmptyMap();
    void            makeWallsAndGates();

    QSize           m_MapSize;
    Map             m_Map;
};



class Player : QObject{
    Q_OBJECT
public:
    Player(int myNr);
    void            addPoint();
    void            resetPoints();
    int             showPoints();
    QString         showName();

private:
    int             m_Score;
    int             m_PlayerNumber;
    QString         m_Name;
};



class PongModel : QObject{
    Q_OBJECT
public:
    PongModel(const int WIDTH, const int HEIGHT);
    PongModel(PongModel &) = delete;
    PongModel & operator=(const PongModel &) = delete;
    const Map &     showMap();
    void            makeNoise(int wallsNumber);
    void            movePaddle(int player, int way);
    void            moveBall();
    void            refresh();
    void            setBasicPositions();
    void            setBasicConfiguration();
    int             showScore(int player);
    QString         showName(int player);
    int             showBallSpeed();
    ~PongModel();

public slots:
    void            firstPlayerAddPoints();
    void            secondPlayerAddPoints();

signals:
    void            sbGetPoints();

private:
    const QSize     GAME_SIZE;
    const QPoint    CENTER_POS;
    Player          * m_FirstPlayer, * m_SecondPlayer;
    Paddle          * m_FirstPaddle, * m_SecondPaddle;
    Ball            * m_Ball;
    Playmap         * m_PlayMap;
};

#endif // PONGMODEL_H
