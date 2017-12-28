#include "pongmodel.h"
#include <stdexcept>
#include <QTime>
#include <QDebug>
#include <QSound>


// BUG PALETKA BOKIEM WCHODZI NA PIŁKĘ

static QTextStream cout(stdout);


////    BALL

void Ball::resetSpeed(){
    m_Speed = 1;
}

const int & Ball::showSpeed() const{
    return m_Speed;
}

void Ball::setPosition(QPoint newPos){
    m_Position = newPos;
}

void Ball::doBallStdReflections(const Map &map, QPoint &newFirst, int &nextPos, QPoint &newPos, QPoint &newSecond)
{
    m_Reflection.x == true ? newPos.setX(newPos.x() + 1) : newPos.setX(newPos.x() - 1);
    m_Reflection.y == true ? newPos.setY(newPos.y() + 1) : newPos.setY(newPos.y() - 1);
    int newX = m_Position.x() - newPos.x();
    int newY = m_Position.y() - newPos.y();

    newFirst.setX(m_Position.x() - newX);
    newFirst.setY(m_Position.y());

    newSecond.setX(m_Position.x());
    newSecond.setY(m_Position.y() - newY);

    int howManyBlocked  = 0;
    int mapFirst        = map[newFirst.y()][newFirst.x()];
    int mapSecond       = map[newSecond.y()][newSecond.x()];

    if(mapFirst > BALL){
        nextPos = mapFirst;
        nextPos = PAD_DEV;
        if(mapFirst == PAD_CENTER)  nextPos = PAD_CENTER;
        if(mapFirst == PAD_BORDER)  nextPos = PAD_BORDER;
        howManyBlocked++;
    }

    if(mapSecond > BALL){
        nextPos = mapSecond;
        nextPos = WALL;
        howManyBlocked++;
    }

    if(howManyBlocked == 0){
        nextPos = map[newPos.y()][newPos.x()];
    }

    if(howManyBlocked == 2){
        m_Reflection.y = !m_Reflection.y;
        m_Reflection.x = !m_Reflection.x;
        newPos = m_Position;
    }
}

void Ball::doBallCollisions(bool & modifier, int nextPos)
{
    switch (nextPos) {
    case FIRST_GATE:
        m_LastHit = EMPTY;
        m_isBallDrunk = false;
        modifier = false;
        resetSpeed();
        QSound::play(":/sounds/Gate.wav");
        emit secondGetPoints();
        break;

    case SECOND_GATE:
        m_LastHit = EMPTY;
        m_isBallDrunk = false;
        modifier = false;
        resetSpeed();
        QSound::play(":/sounds/Gate.wav");
        emit firstGetPoints();
        break;

    case WALL:
        m_LastHit = WALL;
        if(m_isBallDrunk == true) m_LastHit = PAD_DEV;
        QSound::play(":/sounds/Wall.wav");
        m_Reflection.y = !m_Reflection.y;
        break;

    case PAD_CENTER:
        m_LastHit = PAD_CENTER;
        m_isBallDrunk = false;
        QSound::play(":/sounds/Pallet.wav");
        m_Reflection.x = !m_Reflection.x;
        if(m_Speed > 2) m_Speed--;
        break;

    case PAD_DEV:
        m_LastHit = PAD_DEV;
        m_isBallDrunk = false;
        QSound::play(":/sounds/Pallet.wav");
        m_Reflection.x = !m_Reflection.x;
        break;

    case PAD_BORDER:
        m_LastHit = PAD_BORDER;
        m_isBallDrunk = false;
        QSound::play(":/sounds/Pallet.wav");
        m_Reflection.x = !m_Reflection.x;
        m_Speed++;
        break;
    }
}

void Ball::doBallMovement(QPoint & newPos)
{
    m_Reflection.x == true ? m_Position.setY(newPos.y()) : m_Position.setY(newPos.y());
    m_Reflection.y == true ? m_Position.setX(newPos.x()) : m_Position.setX(newPos.x());
}

void Ball::doBallPadCenterReflection(QPoint & newPos)
{
    m_Reflection.x == true ? newPos.setX(newPos.x() + 1) : newPos.setX(newPos.x() - 1);
    m_Reflection.y == true ? newPos.setY(newPos.y()) : newPos.setY(newPos.y());
}

void Ball::doBallDrunkReflection(QPoint & newPos, int & nextPos, QPoint & newFirst, QPoint & newSecond, const Map &map)
{
    m_isBallDrunk = true;
    m_DrunkBallWayFlag = !m_DrunkBallWayFlag;
    if(m_DrunkBallWayFlag == true){
        doBallPadCenterReflection(newPos);
        nextPos = map[newPos.y()][newPos.x()];
    }
    else
        doBallStdReflections(map, newFirst, nextPos, newPos, newSecond);
}

void Ball::moveMe(const Map &map){
    QPoint newFirst, newSecond;
    QPoint newPos   = m_Position;
    bool modifier   = true;
    int nextPos     = EMPTY;

    switch(m_LastHit){
    case PAD_CENTER :
        doBallPadCenterReflection(newPos);
        nextPos = map[newPos.y()][newPos.x()];
        break;

    case PAD_DEV: // liczenie trajektorii
        doBallDrunkReflection(newPos, nextPos, newFirst, newSecond, map);
        break;

    default:    // EMPTY
        doBallStdReflections(map, newFirst, nextPos, newPos, newSecond);
    }

    if(nextPos == EMPTY || nextPos == CENTLINE)
        doBallMovement(newPos);

    else{
         doBallCollisions(modifier, nextPos);
         moveMe(map);
    }
}

const QPoint & Ball::showPos() const{
    return m_Position;
}

Ball::Ball(QPoint myPosition) : m_Position(myPosition){
    m_Reflection.x = true, m_Reflection.y = true;
    m_Speed = 1;
    m_LastHit = EMPTY;
}


////    PADDLE

void Paddle::moveMe(int way){
    int upBorder    = 0;
    int downBorder  = m_MapSize.height();
    if(way == UP){
        if(m_Position.x() - 1 != upBorder)
                m_Position.setX(m_Position.x() - 1);
    }
    else if(way == DOWN){
        if(m_Position.x() + m_PadSize + 1 != downBorder -1)
             m_Position.setX(m_Position.x() + 1);
    }
}

const int &Paddle::mySize()const{
    return m_PadSize;
}

const QPoint &Paddle::myPos()const{
    return m_Position;
}

const QVector<int> & Paddle::myStructure(){
    return m_PadStructure;
}

void Paddle::createPadStructure(){
    for(int & pad : m_PadStructure)
        pad = PAD_DEV;

    int firstFragPos = 0,
        lastFragPos  = m_PadSize - 1;

    m_PadStructure[firstFragPos]    = PAD_BORDER;
    m_PadStructure[lastFragPos]     = PAD_BORDER;

    if(m_PadSize > 4){
        if(m_PadSize % 2 == 0){
            int firstCentPos = m_PadSize / 2;
            int lastCentPos  = firstCentPos - 1;
            m_PadStructure[firstCentPos]    = PAD_CENTER;
            m_PadStructure[lastCentPos]     = PAD_CENTER;
        }
        else{
            int centPos = m_PadSize / 2;
            m_PadStructure[centPos] = PAD_CENTER;
        }
    }
}

Paddle::Paddle(int mySize, QSize mapSize, int givenPlayerSpace): m_PadSize(mySize), m_MapSize(mapSize), m_PadStructure(mySize){
    createPadStructure();
    switch (givenPlayerSpace){
    case 0:
        m_Position = QPoint(m_MapSize.height() / 2 - mySize /2, 1);
        break;
    case 1:
        m_Position = QPoint(m_MapSize.height() / 2 - mySize /2, m_MapSize.width() - 2);
        break;
    default:
        throw std::out_of_range ("There is too many players");
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

void Playmap::makeEmptyMap()
{
    for(int row = 0; row < m_MapSize.height(); row++){
        m_Map.push_back(Row());
        for(int col = 0; col < m_MapSize.width(); col++){
            m_Map.back().push_back(EMPTY);
        }
    }
}

void Playmap::makeCenterLine()
{
    int widCenter = m_MapSize.width()/2;
    for(int lineBloch = 2; lineBloch < m_MapSize.height() -2 ; lineBloch += 2){
        m_Map[lineBloch][widCenter] = CENTLINE;
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
    makeCenterLine();
}

void Playmap::drawMap() const
{
    for(int row = 0; row < m_MapSize.height(); row++){
        for(int col = 0; col < m_MapSize.width(); col++){
            if(m_Map[row][col] == EMPTY)
                cout << " ";
            else if(m_Map[row][col] == BALL)
                cout << "@";
            else if(m_Map[row][col] == PAD_CENTER)
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
    QPoint ballPos = ball->showPos();
    m_Map[ ballPos.y() ][ ballPos.x() ] = BALL;
}

void Playmap::putPaddle(Paddle * paddle){
    QPoint  pos = paddle->myPos();
    const QVector<int> & padStrct = paddle->myStructure();

    int piece = 0;
    for(int givenStrct : padStrct){
        m_Map[ pos.x() + piece ][ pos.y() ] = givenStrct;
        piece++;
    }

}

void Playmap::clearMap(){
    for(Row & myRow : m_Map)
        for(int & val : myRow)
            if( val == BALL || val == PAD_CENTER || val == PAD_BORDER || val == PAD_DEV)
                val = EMPTY;
}

Playmap::Playmap(QSize mapSize) : m_MapSize(mapSize){
    makeEmptyMap();
    makeWallsAndGates();
}



////    PLAYER

const QString &Player::showName() const{
    return m_Name;
}

void Player::resetPoints(){
    m_Score = 0;
}

const int &Player::showPoints()const{
    return m_Score;
}

void Player::addPoint(){
    m_Score++;
}

Player::Player(int myNr) : m_PlayerNumber(myNr){
    m_Name  = QString("%1 Player").arg(m_PlayerNumber + 1);
    m_Score = 0;
}


////    PONG MODEL

const QString & PongModel::showName(int player)const{
    return player == FIRST_PLAYER ? m_FirstPlayer->showName() : m_SecondPlayer->showName();
}

const int &PongModel::showScore(int player)const{
    return player == FIRST_PLAYER ? m_FirstPlayer->showPoints() : m_SecondPlayer->showPoints();
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

    m_FirstPaddle   = new Paddle(PAD_SIZE, GAME_SIZE, FIRST_LOC);
    m_SecondPaddle  = new Paddle(PAD_SIZE, GAME_SIZE, SECOND_LOC);

    refresh();
}
const int & PongModel::showBallSpeed() const{
    return m_Ball->showSpeed();
}

void PongModel::firstPlayerAddPoints(){
    setBasicPositions();
    m_FirstPlayer->addPoint();
    emit sbGetPoints();
}

void PongModel::secondPlayerAddPoints(){
    setBasicPositions();
    m_SecondPlayer->addPoint();
    emit sbGetPoints();
}

void PongModel::moveBall(){
    m_Ball->moveMe(m_PlayMap->showMap());
}

void PongModel::refresh() const{
    m_PlayMap->clearMap();
    m_PlayMap->putBall(m_Ball);
    m_PlayMap->putPaddle(m_FirstPaddle);
    m_PlayMap->putPaddle(m_SecondPaddle);
    m_PlayMap->makeCenterLine();
}

int PongModel::movePaddleSI(){
    int field       = CENTER_POS.x() + CENTER_POS.x() / 6;
    int bPos        = m_Ball->showPos().x();
    bool isSIField  = field < bPos;

    if(isSIField){
        static int oldBallPos   = CENTER_POS.y();
        int ballPos             = m_Ball->showPos().y();
        int diff                = oldBallPos - ballPos;
        oldBallPos              = ballPos;

        if(diff == -1)
            return DOWN;
        else if(diff == 0)
            return NOWHERE;
        else if(diff == 1)
            return UP;
    }
    return NOWHERE;
}

void PongModel::movePaddle(int player, int way){
    switch (player) {
    case FIRST_PLAYER:
        m_FirstPaddle->moveMe(way);
        break;
    case SECOND_PLAYER:
        m_SecondPaddle->moveMe(way);
        break;
    case COMPUTER:
        m_SecondPaddle->moveMe(movePaddleSI());
        break;
    }
}

void PongModel::makeNoise(int wallsNumber){
    m_PlayMap->makeNoise(wallsNumber);
}

const Map & PongModel::showMap() const{
    return m_PlayMap->showMap();
}

PongModel::PongModel(const int WIDTH, const int HEIGHT, QObject * parent) :
    QObject(parent),
    CENTER_POS      (QPoint(WIDTH / 2, HEIGHT / 2)),
    GAME_SIZE       (QSize(WIDTH, HEIGHT)){

    m_PlayMap       = new Playmap(GAME_SIZE);
    m_Ball          = new Ball(CENTER_POS);
    m_FirstPlayer   = new Player(FIRST_PLAYER);
    m_SecondPlayer  = new Player(SECOND_PLAYER);
    m_FirstPaddle   = new Paddle(PAD_SIZE, GAME_SIZE, FIRST_LOC);
    m_SecondPaddle  = new Paddle(PAD_SIZE, GAME_SIZE, SECOND_LOC);

    m_PlayMap->putBall(m_Ball);
    m_PlayMap->putPaddle(m_FirstPaddle);
    m_PlayMap->putPaddle(m_SecondPaddle);

    connect(m_Ball, SIGNAL(firstGetPoints()), this, SLOT(firstPlayerAddPoints()) );
    connect(m_Ball, SIGNAL(secondGetPoints()), this, SLOT(secondPlayerAddPoints()));
}

PongModel::~PongModel(){
    delete m_PlayMap,
           m_Ball,
           m_FirstPlayer,
           m_SecondPlayer,
           m_FirstPaddle,
           m_SecondPaddle;
    }


