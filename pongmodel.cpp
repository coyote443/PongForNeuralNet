#include "pongmodel.h"
#include <stdexcept>
#include <QDebug>

static QTextStream cout(stdout);


////    BALL




bool Ball::moveMe(Map &map){
    QPoint newPos = m_Position;

    m_Reflection.x == true ? newPos.setX(newPos.x() + 1) : newPos.setX(newPos.x() - 1);
    m_Reflection.y == true ? newPos.setY(newPos.y() + 1) : newPos.setY(newPos.y() - 1);

    bool modifier = true;

    if(map[newPos.y()][newPos.x()] == EMPTY){
        m_Reflection.x == true ? m_Position.setY(newPos.y()) : m_Position.setY(newPos.y());
        m_Reflection.y == true ? m_Position.setX(newPos.x()) : m_Position.setX(newPos.x());
    }

    else if(map[newPos.y()][newPos.x()] == GATE)
        modifier = false;

    else{
        if(map[newPos.y()][newPos.x()] == WALL)
            m_Reflection.y = !m_Reflection.y;
        else if(map[newPos.y()][newPos.x()] == PADDLE)
            m_Reflection.x = !m_Reflection.x;
        moveMe(map);
    }

    return (true && modifier);
}

QPoint Ball::myPos(){
    return m_Position;
}

Ball::Ball(QPoint myPos) : m_Position(myPos){
    m_Reflection.x = true, m_Reflection.y = true;
}


////    PADDLE

int Paddle::mySize(){
    return m_Size;
}

QPoint Paddle::myPos(){
    return m_Position;
}

Paddle::Paddle(int mySize, QSize mapSize, int givenSpace): m_Size(mySize){
    switch (givenSpace){
    case 0:
        m_Position = QPoint(mapSize.height() / 2 - mySize /2, 1);
        break;
    case 1:
        m_Position = QPoint(mapSize.height() / 2 - mySize /2, mapSize.width() - 2);
        break;
    default:
        throw std::out_of_range ("There is to many players");
    }
}

////    MAP

void Playmap::makeMap()
{
    for(int row = 0; row < m_MapSize.height(); row++){
        m_Map.push_back(Row());
        for(int col = 0; col < m_MapSize.width(); col++){
            m_Map.back().push_back(EMPTY);
        }
    }
}

void Playmap::makeWallsAndGates()
{
    int wid = m_MapSize.width();
    int hei = m_MapSize.height();

    for(int colNmb = 0; colNmb < wid; colNmb++){
        m_Map[0][colNmb] = WALL;
        m_Map[hei - 1][colNmb] = WALL;
    }

    for(int lineNmb = 0; lineNmb < hei; lineNmb++){
        m_Map[lineNmb][0] = GATE;
        m_Map[lineNmb][wid - 1] = GATE;
    }
}

void Playmap::drawMap() const
{
    for(int row = 0; row < m_MapSize.height(); row++){
        for(int col = 0; col < m_MapSize.width(); col++){
            if(m_Map[row][col] == EMPTY)
                cout << " ";
            else if(m_Map[row][col] == BALL)
                cout << "@";
            else if(m_Map[row][col] == PADDLE)
                cout << "|";
            else if(m_Map[row][col] == WALL)
                cout << "-";
            else
                cout << ".";
        }
        cout << endl;
    }
}

void Playmap::putBall(Ball * ball){
    QPoint ballPos = ball->myPos();
    m_Map[ ballPos.y() ][ ballPos.x() ] = BALL;
}

void Playmap::putPaddle(Paddle * paddle){
    QPoint  pos = paddle->myPos();
    int     size= paddle->mySize();

    for(int piece = 0; piece < size; piece++){
        m_Map[ pos.x() + piece ][ pos.y() ] = PADDLE;
    }

}

void Playmap::clearMap(){
    for(Row & myRow : m_Map)
        for(int & val : myRow)
            if( val == BALL || val == PADDLE)
                val = EMPTY;
}

Map & Playmap::myMap(){
    return m_Map;
}

Playmap::Playmap(QSize mapSize) : m_MapSize(mapSize){
    makeMap();
    makeWallsAndGates();
}



////    PLAYER

Player::Player(QString myName){
    m_Name = myName;
}


////    GAMESPACE

Gamespace::Gamespace(){
    CENTER_POS      = QPoint(WIDTH / 2, HEIGHT / 2);
    GAME_SIZE       = QSize(WIDTH, HEIGHT);

    m_Map           = new Playmap(GAME_SIZE);
    m_Ball          = new Ball(CENTER_POS);
    m_FirstPlayer   = new Player("First Player");
    m_SecondPlayer  = new Player("Second Player");
    m_FirstPaddle   = new Paddle(PADDLE_SIZE, GAME_SIZE, FIRST_LOC);
    m_SecondPaddle  = new Paddle(PADDLE_SIZE, GAME_SIZE, SECOND_LOC);

    while(m_Ball->moveMe(m_Map->myMap()))
    {
        m_Map->clearMap();
        m_Map->putBall(m_Ball);
        m_Map->putPaddle(m_FirstPaddle);
        m_Map->putPaddle(m_SecondPaddle);
        m_Map->drawMap();
    }
//    m_Map->putBall(m_Ball);

}


////    PONG MODEL

PongModel::PongModel(){
    m_Gamespace = new Gamespace;
}


