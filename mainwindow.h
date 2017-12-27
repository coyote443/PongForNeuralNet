#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QSound>

class PongWidget;
class PongModel;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void checkWinner();
    void checkTimer();
    ~MainWindow();

private slots:
    void on_actionStart_triggered();
    void drawBoard();
    void resetTimeStuff();
    void on_actionMakeNoise_triggered();
    void on_actionFirstPlayerUp_triggered();
    void on_actionFirstPlayerDown_triggered();
    void on_actionSecondPlayerUp_triggered();
    void on_actionSecondPlayerDown_triggered();
    void timerEvent();
    void on_actionStop_Music_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    enum {WIDTH = 90, HEIGHT = 30, FILLED = 0, EMPTY = 1, MS_TO_NEXT_EVENT = 80};
    QSize       m_PongBoardSize;
    PongWidget* m_PongWidget;
    PongModel*  m_PongModel;
    QImage      m_PongCurrentImage;
    bool        m_PongIsRunning         = false;
    QTimer*     m_Timer;
    int         m_CurrSpeedModifier;
    int         m_Speed;
    QSound      m_BackgroundMusic;
};

#endif // MAINWINDOW_H
