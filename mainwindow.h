#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QStyle>
#include <QApplication>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QToolButton>
#include <QLCDNumber>
#include <QDateTime>
#include <QLabel>
#include <QFont>

#include "chessboard.h"
#include "qplayer.h"
#include "qgo.h"


QT_BEGIN_NAMESPACE

namespace Ui { class MainWindow; }

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QDesktopWidget dw;
    QStyle *style = QApplication::style();

    ChessBoard *ChessBoard;
    QWidget *SideBar;
    QLCDNumber *TotalMoves;

    int SideBarIconSize = 35;
    int SideBarButtonSize = 50;

    QAction *NewFile;
    QAction *OpenFile;
    QAction *SaveFile;
    QAction *Exit;
    QAction *StartGame;
    QAction *PauseGame;
    QAction *StopGame;
    QAction *NextMove;
    QAction *PreviousMove;
    QAction *TenNextMove;
    QAction *TenPreviousMove;
    QAction *LastMove;
    QAction *FirstMove;

    QPlayer *WhitePlayer;
    QPlayer *BlackPlayer;

    QGo *MainGame;

    void CreateMenu();
    void CreateSideBar();
signals:
    void Refresh();
public slots:
    void TimerUpdate(void);
    void GameGuide(void);
    void AboutGame(void);
    void New();
};
#endif // MAINWINDOW_H
