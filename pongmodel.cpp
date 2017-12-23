#include "pongmodel.h"
#include <stdexcept>
#include <QTime>
#include <QDebug>

static QTextStream cout(stdout);


////    BALL

void Ball::setPosition(QPoint newPos){
    m_Position = newPos;
}

bool Ball::moveMe(const Map &map){
    QPoint newPos = m_Position;

    m_Reflection.x == true ? newPos.setX(newPos.x() + 1) : newPos.setX(newPos.x() - 1);
    m_Reflection.y == true ? newPos.setY(newPos.y() + 1) : newPos.setY(newPos.y() - 1);

    bool modifier = true;

    if(map[newPos.y()][newPos.x()] == EMPTY){
        m_Reflection.x == true ? m_Position.setY(newPos.y()) : m_Position.setY(newPos.y());
        m_Reflection.y == true ? m_Position.setX(newPos.x()) : m_Position.setX(newPos.x());
    }

    else if(map[newPos.y()][newPos.x()] == FIRST_GATE || map[newPos.y()][newPos.x()] == SECOND_GATE){
        modifier = false;
        if(map[newPos.y()][newPos.x()] == FIRST_GATE)
            emit secondGetPoints();
        else
            emit firstGetPoints();
    }

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

void Paddle::moveMe(bool upTrueDownFalse){
    int upBorder    = 0;
    int downBorder  = m_MapSize.height();

    if(upTrueDownFalse == true){
        if(m_Position.x() - 1 != upBorder)
                m_Position.setX(m_Position.x() - 1);
    }
    else{
        if(m_Position.x() + m_Size + 1 != downBorder)
             m_Position.setX(m_Position.x() + 1);
    }
}

int Paddle::mySize(){
    return m_Size;
}

QPoint Paddle::myPos(){
    return m_Position;
}

Paddle::Paddle(int mySize, QSize mapSize, int givenSpace): m_Size(mySize), m_MapSize(mapSize){
    switch (givenSpace){
    case 0:
        m_Position = QPoint(m_MapSize.height() / 2 - mySize /2, 1);
        break;
    case 1:
        m_Position = QPoint(m_MapSize.height() / 2 - mySize /2, m_MapSize.width() - 2);
        break;
    default:
        throw std::out_of_range ("There is to many players");
    }
}

////    PLAYMAP

void Playmap::makeNoise(int wallsNumber){
    qsrand(QTime::currentTime().msec());

    int counter = 0;

    while(counter < wallsNumber){
        int wid = m_MapSize.width();
        int hei = m_MapSize.height();
        int rx = qrand() % (wid - wid / 5) + wid / 10;
        int ry = qrand() % hei;

        if(m_Map[ry][rx] == EMPTY){
            m_Map[ry][rx] = WALL;
            counter++;
        }
    }
}

const Map &Playmap::showMap() const{
    return m_Map;
}

void Playmap::makeMap()
{
    for(int row = 0; row < m_MapSize.height(); row++){
        m_Map.push_back(Row());
        for(int col = 0; col < m_MapSize.width(); col++){
            m_Map.back().push_back(EMPTY);
        }
    }
}

void Playmap::makeWallsAndGates() // fix those dirty shit
{
    int wid = m_MapSize.width();
    int hei = m_MapSize.height();

    for(int colNmb = 0; colNmb < wid; colNmb++){
        m_Map[0][colNmb] = WALL;
        m_Map[hei - 1][colNmb] = WALL;
        m_Map[hei - 2][colNmb] = WALL;      // fix those dirty shit
    }

    for(int lineNmb = 0; lineNmb < hei; lineNmb++){
        m_Map[lineNmb][0] = FIRST_GATE;
        m_Map[lineNmb][wid - 1] = SECOND_GATE;
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

Playmap::Playmap(QSize mapSize) : m_MapSize(mapSize){
    makeMap();
    makeWallsAndGates();
}



////    PLAYER

QString Player::showName(){
    return m_Name;
}

void Player::resetPoints(){
    m_Score = 0;
}

int Player::showPoints(){
    return m_Score;
}

void Player::addPoint(){
    m_Score++;
}

Player::Player(int myNr) : m_PlayerNumber(myNr){
    m_Name  = QString("%1 Player").arg(m_PlayerNumber);
    m_Score = 0;
}


////    PONG MODEL

QString PongModel::showName(int player){
    return player == FIRST_P ? m_FirstPlayer->showName() : m_SecondPlayer->showName();
}

int PongModel::showScore(int player){
    return player == FIRST_P ? m_FirstPlayer->showPoints() : m_SecondPlayer->showPoints();
}

void PongModel::setBasicConfiguration(){
    m_FirstPlayer->resetPoints();
    m_SecondPlayer->resetPoints();
    setBasicPositions();
}

void PongModel::setBasicPositions(){
    m_Ball->setPosition(CENTER_POS);

    delete m_FirstPaddle;
    delete m_SecondPaddle;

    m_FirstPaddle   = new Paddle(PADDLE_SIZE, GAME_SIZE, FIRST_LOC);
    m_SecondPaddle  = new Paddle(PADDLE_SIZE, GAME_SIZE, SECOND_LOC);

    refresh();
}

void PongModel::firstPlayerAddPoints(){
    setBasicPositions();
    m_FirstPlayer->addPoint();
}

void PongModel::secondPlayerAddPoints(){
    setBasicPositions();
    m_SecondPlayer->addPoint();
}

void PongModel::moveBall(){
    m_Ball->moveMe(m_PlayMap->showMap());
}

void PongModel::refresh(){
    m_PlayMap->clearMap();
    m_PlayMap->putBall(m_Ball);
    m_PlayMap->putPaddle(m_FirstPaddle);
    m_PlayMap->putPaddle(m_SecondPaddle);
}

void PongModel::movePaddle(int player, int way){
    player == FIRST_P ? m_FirstPaddle->moveMe(way) : m_SecondPaddle->moveMe(way);
}

void PongModel::makeNoise(int wallsNumber){
    m_PlayMap->makeNoise(wallsNumber);
}

const Map & PongModel::showMap(){
    return m_PlayMap->showMap();
}

PongModel::PongModel(const int WIDTH, const int HEIGHT) :
    CENTER_POS      (QPoint(WIDTH / 2, HEIGHT / 2)),
    GAME_SIZE       (QSize(WIDTH, HEIGHT)){

    m_PlayMap       = new Playmap(GAME_SIZE);
    m_Ball          = new Ball(CENTER_POS);
    m_FirstPlayer   = new Player(FIRST_P);
    m_SecondPlayer  = new Player(SECOND_P);
    m_FirstPaddle   = new Paddle(PADDLE_SIZE, GAME_SIZE, FIRST_LOC);
    m_SecondPaddle  = new Paddle(PADDLE_SIZE, GAME_SIZE, SECOND_LOC);

    m_PlayMap->putBall(m_Ball);
    m_PlayMap->putPaddle(m_FirstPaddle);
    m_PlayMap->putPaddle(m_SecondPaddle);

    connect((const QObject *)m_Ball, SIGNAL(firstGetPoints()), this, SLOT(firstPlayerAddPoints()) );
    connect((const QObject *)m_Ball, SIGNAL(secondGetPoints()), this, SLOT(secondPlayerAddPoints()));
}

PongModel::~PongModel(){
    delete m_PlayMap;
    m_PlayMap       = nullptr;
    delete m_Ball;
    m_Ball          = nullptr;
    delete m_FirstPlayer;
    m_FirstPlayer   = nullptr;
    delete m_SecondPlayer;
    m_SecondPlayer  = nullptr;
    delete m_FirstPaddle;
    m_FirstPaddle   = nullptr;
    delete m_SecondPaddle;
    m_SecondPaddle  = nullptr;
}


