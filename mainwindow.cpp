#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pongwidget.h"
#include "pongmodel.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <QIcon>
#include <QDebug>

static QTextStream cout(stdout);


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_BackgroundMusic(":/sounds/ACDC.wav")
{
    m_BackgroundMusic.play();

    ui->setupUi(this);
    m_PongWidget    = new PongWidget(this);
    m_PongModel     = new PongModel(WIDTH, HEIGHT);
    m_Timer         = new QTimer(this);

    m_PongWidget->setFixedSize(800,470);
    m_PongWidget->move(10, 120);

    ui->GameWidget = m_PongWidget;

    m_PongBoardSize = QSize(WIDTH, HEIGHT);
    m_PongCurrentImage = QImage(m_PongBoardSize, QImage::Format_Mono);
    m_PongCurrentImage.fill(FILLED);
    m_PongWidget->setImage(m_PongCurrentImage);

    ui->ScoreToWin->display(POINTS_TO_WIN);

    connect((QObject *)m_PongModel, SIGNAL(sbGetPoints()), this, SLOT(resetTimeStuff()) );
    connect(m_Timer, SIGNAL(timeout()), this, SLOT(timerEvent()) );

    m_Timer->start(MS_TO_NEXT_EVENT);

    m_CurrSpeedModifier = m_PongModel->showBallSpeed();
    m_Speed = MS_TO_NEXT_EVENT;

    ui->actionStart->setIcon(QIcon(":/icos/StartGame.ico"));
    ui->actionMakeNoise->setIcon(QIcon(":/icos/AddNoise.ico"));
    ui->actionStop_Music->setIcon(QIcon(":/icos/SoundON.ico"));

    drawBoard();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::checkTimer(){
    int ballSpeed = m_PongModel->showBallSpeed();

    if(m_CurrSpeedModifier == ballSpeed) return;

    if(m_CurrSpeedModifier < ballSpeed){
        m_Speed -= 20;
        if(m_Speed < 20) m_Speed = 1;
        m_Timer->setInterval(m_Speed);
    }

    else{
        if(m_Speed * m_CurrSpeedModifier < MS_TO_NEXT_EVENT){
            m_Speed *= m_CurrSpeedModifier;
            m_Timer->setInterval(m_Speed);
        }
    }

    m_CurrSpeedModifier = ballSpeed;
}

void MainWindow::resetTimeStuff(){
    m_Timer->setInterval(MS_TO_NEXT_EVENT);
    m_Speed = MS_TO_NEXT_EVENT;
    m_CurrSpeedModifier = m_PongModel->showBallSpeed();
}


void MainWindow::timerEvent(){
    if(m_PongIsRunning == true){
        qApp->processEvents();
        m_PongModel->moveBall();
        m_PongModel->refresh();
        drawBoard();
        checkTimer();
        checkWinner();
    }
}


void MainWindow::on_actionStart_triggered(){
    m_PongIsRunning = !m_PongIsRunning;
    drawBoard();
}

void MainWindow::checkWinner(){
    int frstScore = m_PongModel->showScore(FIRST_PLAYER);
    int scndScore = m_PongModel->showScore(SECOND_PLAYER);

    if(frstScore == POINTS_TO_WIN || scndScore == POINTS_TO_WIN){
        QString WinnerName = (frstScore == POINTS_TO_WIN ? m_PongModel->showName(FIRST_PLAYER) : m_PongModel->showName(SECOND_PLAYER));
        QMessageBox::StandardButton butt;

        butt = QMessageBox::question(0, "Wygrana", QString("Wygrał %1 czy chcesz grać dalej?").arg(WinnerName),
                                     QMessageBox::Yes | QMessageBox::No);

        if(butt == QMessageBox::Yes){
            m_PongIsRunning = false;
            m_PongModel->setBasicConfiguration();
        }
        else
            QApplication::quit();
    }

}

void MainWindow::drawBoard(){
    ui->ScoreFirstPlayer->display(m_PongModel->showScore(FIRST_PLAYER));
    ui->ScoreSecondPlayer->display(m_PongModel->showScore(SECOND_PLAYER));

    QImage next = m_PongCurrentImage;
    const Map & map = m_PongModel->showMap();

    for(int row = 0; row < map.size(); row++){
        for(int col = 0; col < map.back().size(); col++){
            map[row][col] > 0 ? next.setPixel(col, row, FILLED) : next.setPixel(col, row, EMPTY);
        }
    }
    m_PongCurrentImage = next;
    m_PongWidget->setImage(m_PongCurrentImage);
}

void MainWindow::on_actionMakeNoise_triggered()
{
    m_PongIsRunning = false;
    int howManyWalls = QInputDialog::getInt(0, "Noise", "How many walls?", 0, 0, 500);
    m_PongModel->makeNoise(howManyWalls);
    drawBoard();
}

void MainWindow::on_actionFirstPlayerUp_triggered(){
    if(m_PongIsRunning){
        m_PongModel->movePaddle(FIRST_PLAYER, UP);
        m_PongModel->refresh();
        drawBoard();
    }
}

void MainWindow::on_actionFirstPlayerDown_triggered(){
    if(m_PongIsRunning){
        m_PongModel->movePaddle(FIRST_PLAYER, DOWN);
        m_PongModel->refresh();
        drawBoard();
    }
}

void MainWindow::on_actionSecondPlayerUp_triggered(){
    if(m_PongIsRunning){
        m_PongModel->movePaddle(SECOND_PLAYER, UP);
        m_PongModel->refresh();
        drawBoard();
    }
}

void MainWindow::on_actionSecondPlayerDown_triggered(){
    if(m_PongIsRunning){
        m_PongModel->movePaddle(SECOND_PLAYER, DOWN);
        m_PongModel->refresh();
        drawBoard();
    }
}

void MainWindow::on_actionStop_Music_triggered(bool checked){
    if(checked){
        m_BackgroundMusic.stop();
        ui->actionStop_Music->setIcon(QIcon(":/icos/SoundOFF.ico"));
        ui->actionStop_Music->setText("Play Music");
    }
    else{
        m_BackgroundMusic.play();
        ui->actionStop_Music->setIcon(QIcon(":/icos/SoundON.ico"));
        ui->actionStop_Music->setText("Stop Music");
    }
}

