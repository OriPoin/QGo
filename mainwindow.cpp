#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMouseTracking(true);
    int x = 0;
    int y = 0;
    if (dw.width() > 1300)
    {
        x = (dw.width() - 1300) / 2;
    }
    if (dw.height() > 820)
    {
        y = (dw.height() - 820) / 2;
    }
    WhitePlayer = new class QPlayer(this);
    BlackPlayer = new class QPlayer(this);
    MainGame = new class QGo(this);

    this->setWindowTitle("QGo");
    this->setWindowIcon(QIcon(":/image/logo.png"));
    this->move(x,y);
    this->setFixedSize(1300, 820);

    ChessBoard = new class ChessBoard(this);
    CreateMenu();
    CreateSideBar();

    QTimer *Timer = new QTimer(this);

    this->connect(MainGame,&QGo::valueChanged,ChessBoard,&ChessBoard::SetColor);
    this->connect(ChessBoard,&ChessBoard::Placed,MainGame,&QGo::NextMove);
    this->connect(ChessBoard,SIGNAL(Placed(int,int,int)),MainGame,SLOT(SetNode(int,int,int)));
    this->connect(ChessBoard,&ChessBoard::Placed,ChessBoard,&ChessBoard::StateLoop);
    this->connect(this,&MainWindow::Refresh,ChessBoard,&ChessBoard::Refresh);
    this->connect(this,&MainWindow::Refresh,MainGame,&QGo::InitMove);
    connect(Timer,SIGNAL(timeout()),this,SLOT(TimerUpdate()));
    Timer->start(40);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::TimerUpdate(void)
{
    //ui->TotalMoves->display(MainGame->TotalMoveNum());
    TotalMoves->display(MainGame->TotalMoveNum());
    ChessBoard->update();
    ChessBoard->SetColor(MainGame->TotalMoveNum()%2);
}

void MainWindow::CreateMenu()
{
    QMenu *FileMenu = menuBar()->addMenu("File");

    QIcon OpenIcon = style->standardIcon(QStyle::SP_DialogOpenButton);
    QIcon SaveIcon = style->standardIcon(QStyle::SP_DialogSaveButton);
    QIcon ExitIcon = style->standardIcon(QStyle::SP_BrowserStop);
    QIcon NewFileIcon = style->standardIcon(QStyle::SP_FileIcon);

    QAction *NewFile = new QAction(NewFileIcon, "New", FileMenu);
    QAction *OpenFile = new QAction(OpenIcon, "Open", FileMenu);
    QAction *SaveFile = new QAction(SaveIcon, "Save", FileMenu);
    QAction *Exit = new QAction(ExitIcon, "Exit", FileMenu);

    FileMenu->addAction(OpenFile);
    FileMenu->addAction(NewFile);
    FileMenu->addAction(SaveFile);
    FileMenu->addAction(Exit);

    this->connect(Exit,SIGNAL(triggered()),this,SLOT(close()));
    this->connect(NewFile,&QAction::triggered,this,&MainWindow::New);
    this->connect(SaveFile,&QAction::triggered,MainGame,&QGo::SaveSGF);

    QMenu *GameMenu = menuBar()->addMenu("Game");
    QIcon StartButtonIcon = style->standardIcon(QStyle::SP_MediaPlay);
    QIcon PauseButtonIcon = style->standardIcon(QStyle::SP_MediaPause);
    QIcon StopButtonIcon = style->standardIcon(QStyle::SP_MediaStop);
    QIcon *NextMoveIcon = new QIcon(":/image/next.png");
    QIcon *PreviousMoveIcon = new QIcon(":/image/previous.png");
    QIcon *TenNextMoveicon = new QIcon(":/image/10movesforward.png");
    QIcon *TenPreviousMOveicon = new QIcon(":/image/10movesback.png");
    QIcon *LastMoveIcon = new QIcon(":/image/last.png");
    QIcon *FirstMoveIcon = new QIcon(":/image/first.png");

    QAction *StartGame = new QAction(StartButtonIcon,"Start",GameMenu);
    QAction *PauseGame = new QAction(PauseButtonIcon,"Pause",GameMenu);
    QAction *StopGame = new QAction(StopButtonIcon,"Stop",GameMenu);
    QAction *NextMove = new QAction(*NextMoveIcon,"Next",GameMenu);
    QAction *PreviousMove = new QAction(*PreviousMoveIcon,"Previous",GameMenu);
    QAction *TenNextMove = new QAction(*TenNextMoveicon,"10 Moves Forward",GameMenu);
    QAction *TenPreviousMove = new QAction(*TenPreviousMOveicon,"10 Moves Back",GameMenu);
    QAction *LastMove = new QAction(*LastMoveIcon,"Last Move",GameMenu);
    QAction *FirstMove = new QAction(*FirstMoveIcon,"First Move",GameMenu);

    GameMenu->addAction(StartGame);
    GameMenu->addAction(PauseGame);
    GameMenu->addAction(StopGame);
    GameMenu->addSeparator();
    GameMenu->addAction(NextMove);
    GameMenu->addAction(PreviousMove);
    GameMenu->addAction(TenNextMove);
    GameMenu->addAction(TenPreviousMove);
    GameMenu->addAction(LastMove);
    GameMenu->addAction(FirstMove);

    QMenu *HelpMenu = menuBar()->addMenu("Help");

    QAction *GameGuideAct = new QAction("How to play");
    QAction *AboutAct = new QAction("About");

    HelpMenu->addAction(GameGuideAct);
    HelpMenu->addAction(AboutAct);

    this->connect(GameGuideAct,SIGNAL(triggered()),this,SLOT(GameGuide()));
    this->connect(AboutAct,SIGNAL(triggered()),this,SLOT(AboutGame()));

    this->connect(NextMove,&QAction::triggered,MainGame,&QGo::NextMove);
    this->connect(PreviousMove,&QAction::triggered,MainGame,&QGo::PreviousMove);
    this->connect(TenNextMove,&QAction::triggered,MainGame,&QGo::TenNextMove);
    this->connect(TenPreviousMove,&QAction::triggered,MainGame,&QGo::TenPreviousMove);
    this->connect(LastMove,&QAction::triggered,MainGame,&QGo::LastMove);
    this->connect(FirstMove,&QAction::triggered,MainGame,&QGo::FirstMove);
}
void MainWindow::CreateSideBar()
{
    QWidget *SideBar = new QWidget(this);
    SideBar->move(800,22);
    SideBar->setFixedSize(500,800);
    SideBar->setStyleSheet("background-color:#ddd;");

    QIcon *NextMoveIcon = new QIcon(":/image/next.png");
    QIcon *PreviousMoveIcon = new QIcon(":/image/previous.png");
    QIcon *TenNextMoveicon = new QIcon(":/image/10movesforward.png");
    QIcon *TenPreviousMOveicon = new QIcon(":/image/10movesback.png");
    QIcon *LastMoveIcon = new QIcon(":/image/last.png");
    QIcon *FirstMoveIcon = new QIcon(":/image/first.png");

    QToolButton *FirstMoveButton = new QToolButton(SideBar);
    FirstMoveButton->setFixedSize(SideBarButtonSize,SideBarButtonSize);
    FirstMoveButton->setIconSize(QSize(SideBarIconSize,SideBarIconSize));
    FirstMoveButton->move(0,0);
    FirstMoveButton->triggered(FirstMove);
    FirstMoveButton->setIcon(*FirstMoveIcon);

    QToolButton *TenMOveBackButton = new QToolButton(SideBar);
    TenMOveBackButton->setFixedSize(SideBarButtonSize,SideBarButtonSize);
    TenMOveBackButton->setIconSize(QSize(SideBarIconSize,SideBarIconSize));
    TenMOveBackButton->move(60,0);
    TenMOveBackButton->triggered(TenNextMove);
    TenMOveBackButton->setIcon(*TenPreviousMOveicon);

    QToolButton *PreviousMoveButton = new QToolButton(SideBar);
    PreviousMoveButton->setFixedSize(SideBarButtonSize,SideBarButtonSize);
    PreviousMoveButton->setIconSize(QSize(SideBarIconSize,SideBarIconSize));
    PreviousMoveButton->move(120,0);
    PreviousMoveButton->triggered(PreviousMove);
    PreviousMoveButton->setIcon(*PreviousMoveIcon);

    TotalMoves = new QLCDNumber(3,SideBar);
    TotalMoves->setFixedSize(120,50);
    TotalMoves->move(185,0);

    QToolButton *NextMoveToolButton = new QToolButton(SideBar);
    NextMoveToolButton->setFixedSize(SideBarButtonSize,SideBarButtonSize);
    NextMoveToolButton->setIconSize(QSize(SideBarIconSize,SideBarIconSize));
    NextMoveToolButton->move(320,0);
    NextMoveToolButton->setIcon(*NextMoveIcon);

    QToolButton *TenMoveForwardButton = new QToolButton(SideBar);
    TenMoveForwardButton->setFixedSize(SideBarButtonSize,SideBarButtonSize);
    TenMoveForwardButton->setIconSize(QSize(SideBarIconSize,SideBarIconSize));
    TenMoveForwardButton->move(380,0);
    TenMoveForwardButton->setIcon(*TenNextMoveicon);

    QToolButton *LastMoveButton = new QToolButton(SideBar);
    LastMoveButton->setFixedSize(SideBarButtonSize,SideBarButtonSize);
    LastMoveButton->setIconSize(QSize(SideBarIconSize,SideBarIconSize));
    LastMoveButton->move(440,0);
    LastMoveButton->setIcon(*LastMoveIcon);

    this->connect(NextMoveToolButton,&QToolButton::clicked,MainGame,&QGo::NextMove);
    this->connect(PreviousMoveButton,&QToolButton::clicked,MainGame,&QGo::PreviousMove);
    this->connect(TenMoveForwardButton,&QToolButton::clicked,MainGame,&QGo::TenNextMove);
    this->connect(TenMOveBackButton,&QToolButton::clicked,MainGame,&QGo::TenPreviousMove);
    this->connect(LastMoveButton,&QToolButton::clicked,MainGame,&QGo::LastMove);
    this->connect(FirstMoveButton,&QToolButton::clicked,MainGame,&QGo::FirstMove);

    QWidget *WhitePlayerInfo = new QWidget(SideBar);
    WhitePlayerInfo->move(0,51);
    WhitePlayerInfo->setFixedSize(500,350);
    QLabel *WhitePlayerNmae = new QLabel("WhitePlayer",WhitePlayerInfo);
    QLabel *WhitePieces = new QLabel(WhitePlayerInfo);
    WhitePieces->setPixmap(QPixmap(":/image/white.png"));
    WhitePieces->move(448,0);
    WhitePlayerNmae->setFont(QFont("Times", 25, QFont::Bold));

    QLabel *WhitePlayerRank = new QLabel("Rank\n",WhitePlayerInfo);
    WhitePlayerRank->move(5,50);
    WhitePlayerRank->setFixedSize(150,95);
    WhitePlayerRank->setAlignment(Qt::AlignCenter);
    WhitePlayerRank->setFont(QFont("Times", 15, QFont::Bold));
    WhitePlayerRank->setStyleSheet("background-color:#fff;border-radius:15px;");

    QLabel *WhitePlayerCaps = new QLabel("Caps\n",WhitePlayerInfo);
    WhitePlayerCaps->move(172,50);
    WhitePlayerCaps->setFixedSize(150,95);
    WhitePlayerCaps->setAlignment(Qt::AlignCenter);
    WhitePlayerCaps->setFont(QFont("Times", 15, QFont::Bold));
    WhitePlayerCaps->setStyleSheet("background-color:#fff;border-radius:15px;");

    QLabel *WhitePlayerTime = new QLabel("Time\n",WhitePlayerInfo);
    WhitePlayerTime->move(340,50);
    WhitePlayerTime->setFont(QFont("Times", 15, QFont::Bold));
    WhitePlayerTime->setFixedSize(150,95);
    WhitePlayerTime->setAlignment(Qt::AlignCenter);
    WhitePlayerTime->setStyleSheet("background-color:#fff;border-radius:15px;");

    QWidget *BlackPlayerInfo = new QWidget(SideBar);
    BlackPlayerInfo->move(0,200);
    BlackPlayerInfo->setFixedSize(500,350);
    QLabel *BlackPlayerNmae = new QLabel("BlackPlayer",BlackPlayerInfo);
    QLabel *BlackPieces = new QLabel(BlackPlayerInfo);
    BlackPieces->setPixmap(QPixmap(":/image/black.png"));
    BlackPieces->move(448,0);
    BlackPlayerNmae->setFont(QFont("Times", 25, QFont::Bold));

    QLabel *BlackPlayerRank = new QLabel("Rank\n",BlackPlayerInfo);
    BlackPlayerRank->move(5,50);
    BlackPlayerRank->setFixedSize(150,95);
    BlackPlayerRank->setAlignment(Qt::AlignCenter);
    BlackPlayerRank->setFont(QFont("Times", 15, QFont::Bold));
    BlackPlayerRank->setStyleSheet("background-color:#fff;border-radius:15px;");

    QLabel *BlackPlayerCaps = new QLabel("Caps\n",BlackPlayerInfo);
    BlackPlayerCaps->move(172,50);
    BlackPlayerCaps->setFixedSize(150,95);
    BlackPlayerCaps->setAlignment(Qt::AlignCenter);
    BlackPlayerCaps->setFont(QFont("Times", 15, QFont::Bold));
    BlackPlayerCaps->setStyleSheet("background-color:#fff;border-radius:15px;");

    QLabel *BlackPlayerTime = new QLabel("Time\n",BlackPlayerInfo);
    BlackPlayerTime->move(340,50);
    BlackPlayerTime->setFont(QFont("Times", 15, QFont::Bold));
    BlackPlayerTime->setFixedSize(150,95);
    BlackPlayerTime->setAlignment(Qt::AlignCenter);
    BlackPlayerTime->setStyleSheet("background-color:#fff;border-radius:15px;");

    QWidget *Comment = new QWidget(SideBar);
    Comment->move(0,350);
    QLabel *CommentName = new QLabel("Comment",Comment);
    CommentName->setFont(QFont("Times", 25, QFont::Bold));
    QLabel *CommentContent = new QLabel(Comment);
    CommentContent->move(10,40);
    CommentContent->setFixedSize(480,400);
    CommentContent->setStyleSheet("background-color:#fff;border-radius:15px;");

}

void MainWindow::GameGuide()
{
    QMessageBox::about(this,"How To Play","The two players, Black and White, take turns placing stones of their colour on the intersections of the board, one stone at a time. The usual board size is a 19×19 grid");
}

void MainWindow::AboutGame()
{
    QMessageBox::about(this,"About","A Go hess Game based on Qt");
}
void MainWindow::New()
{
    emit Refresh();
}
