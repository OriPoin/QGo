#include "chessboard.h"

ChessBoard::ChessBoard(QWidget *parent) : QWidget(parent)
{
    this->move(0,21);
    this->setFixedSize(800,800);
    for (int i=0;i<=18;i++) {
        for (int j=0;j<=18;j++) {
            BoardGride[i][j]=0;
            BoardPreview[i][j]=0;
        }
    }
    //BoardGride[9][9]=1;
    tempx=0;
    tempy=0;
    color=2;
    BlackScore=0;
    WhiteScore=0;
    this->setMouseTracking(true);
}
void ChessBoard::paintEvent(QPaintEvent *event)
{
    QPainter *Painter = new QPainter(this);

    Painter->begin(this);
    Painter->setBrush(QBrush(Qt::black));
    Painter->setRenderHint(QPainter::Antialiasing, true);

    QPixmap *BoardImage = new QPixmap(":/image/checkerboard.png");
    BoardImage->scaled(QSize(800,800),Qt::KeepAspectRatio);
    Painter->drawPixmap(QRect(0,0,800,800),*BoardImage);
    for (int i=1;i<=19;i++) {
        Painter->drawLine(40*i,40,40*i,760);
        Painter->drawLine(40,40*i,760,40*i);
    }
    Painter->drawEllipse(160-5,160-5,10,10);
    Painter->drawEllipse(400-5,160-5,10,10);
    Painter->drawEllipse(640-5,160-5,10,10);
    Painter->drawEllipse(160-5,400-5,10,10);
    Painter->drawEllipse(400-5,400-5,10,10);
    Painter->drawEllipse(640-5,400-5,10,10);
    Painter->drawEllipse(160-5,640-5,10,10);
    Painter->drawEllipse(400-5,640-5,10,10);
    Painter->drawEllipse(640-5,640-5,10,10);
    for (int i=0;i<=18;i++) {
        for (int j=0;j<=18;j++) {
            if(BoardGride[i][j]==1)
            {
                Painter->drawPixmap(QRect(40*(i+1)-15,40*(j+1)-15,30,30),QPixmap(":/image/white.png"));
            }
            if(BoardGride[i][j]==2)
            {
                Painter->drawPixmap(QRect(40*(i+1)-15,40*(j+1)-15,30,30),QPixmap(":/image/black.png"));
            }
            if(BoardPreview[i][j]==1)
            {
                Painter->setOpacity(0.5);
                Painter->drawPixmap(QRect(40*(i+1)-15,40*(j+1)-15,30,30),QPixmap(":/image/white.png"));
                Painter->setOpacity(1);
            }
            if(BoardPreview[i][j]==2)
            {
                Painter->setOpacity(0.5);
                Painter->drawPixmap(QRect(40*(i+1)-15,40*(j+1)-15,30,30),QPixmap(":/image/black.png"));
                Painter->setOpacity(1);
            }
        }
    }


    Painter->end();
}

void ChessBoard::mouseMoveEvent(QMouseEvent *event)
{
    QPoint MousePosition = event->pos();
    int blockx=MousePosition.x()/40;
    int blocky=MousePosition.y()/40;
    for (int i=0;i<=18;i++) {
        for (int j=0;j<=18;j++) {
            BoardPreview[i][j]=0;
        }
    }
    if(blockx!=0&&blockx!=19&&blocky!=0&&blocky!=19)
    {
        if(MousePosition.x()-40*blockx<=20)
        {
            if(MousePosition.y()-40*blocky<=20)
            {
                BoardPreview[blockx-1][blocky-1]=(BoardGride[blockx-1][blocky-1]==0)?color:BoardGride[blockx-1][blocky-1];
            }
            else
            {
                BoardPreview[blockx-1][blocky]=(BoardGride[blockx-1][blocky]==0)?color:BoardGride[blockx-1][blocky];
            }
        }
        else
        {
            if(MousePosition.y()-40*blocky<=20)
            {
                BoardPreview[blockx][blocky-1]=(BoardGride[blockx][blocky-1]==0)?color:BoardGride[blockx][blocky-1];
            }
            else
            {
                BoardPreview[blockx][blocky]=(BoardGride[blockx][blocky]==0)?color:BoardGride[blockx][blocky];
            }
        }
    }
}
void ChessBoard::Refresh()
{
    for (int i=0;i<=18;i++) {
        for (int j=0;j<=18;j++) {
            PointState[i][j]=Unchecked;
            BoardGride[i][j]=0;
        }
    }
}
void ChessBoard::mousePressEvent(QMouseEvent *event)
{
    QPoint MousePosition = event->pos();
    int blockx=MousePosition.x()/40;
    int blocky=MousePosition.y()/40;

    if(blockx!=0&&blockx!=19&&blocky!=0&&blocky!=19)
    {
        if(MousePosition.x()-40*blockx<=20)
        {
            if(MousePosition.y()-40*blocky<=20)
            {
                this->Place(color,blockx-1,blocky-1);
            }
            else
            {
                this->Place(color,blockx-1,blocky);
            }
        }
        else
        {
            if(MousePosition.y()-40*blocky<=20)
            {
                this->Place(color,blockx,blocky-1);
            }
            else
            {
                this->Place(color,blockx,blocky);
            }
        }
    }
}

void ChessBoard::SetColor(int color)
{
    if(color%2==0)
    {
        this->color=2;//black
    }
    else
    {
        this->color=1;//white
    }
}
void ChessBoard::Place(int color,int x, int y)
{
    if(BoardGride[x][y]==0)
    {
        BoardGride[x][y]=color;
        emit Placed(color,x,y);
    }
}

void ChessBoard::StateLoop()
{
    for (int i=0;i<=18;i++) {
        for (int j=0;j<=18;j++) {
            PointState[i][j]=Unchecked;
        }
    }
    for (int i=0;i<=18;i++) {
        for (int j=0;j<=18;j++) {
            if(PointState[i][j]==Unchecked)
            {
                if(BoardGride[i][j]==0)
                {
                    PointState[i][j] = CheckLiberty(i,j);
                    if(PointState[i][j]==await)
                    {
                        SetLiberty(FreeLiberty,i,j);
                    }
                    if(PointState[i][j]==White)
                    {
                        SetLiberty(WhiteLiberty,i,j);
                    }
                    if(PointState[i][j]==Black)
                    {
                        SetLiberty(BlackLiberty,i,j);
                    }
                }
            }
        }
    }
    for (int i=0;i<=18;i++) {
        for (int j=0;j<=18;j++) {
            if(PointState[i][j]==Unchecked)
            {
                PointState[i][j]=Juge(i,j);
            }
        }
    }
}

ChessBoard::State ChessBoard::CheckLiberty(int x, int y)
{

    PointState[x][y]=await;

    ChessBoard::State StateUp = CheckState(x,y-1);
    StateUp==(StateUp==Unchecked)?CheckLiberty(x,y-1):StateUp;

    ChessBoard::State StateDown = CheckState(x,y+1);
    StateDown==(StateDown==Unchecked)?CheckLiberty(x,y+1):StateDown;

    ChessBoard::State StateLeft = CheckState(x-1,y);
    StateLeft=(StateLeft==Unchecked)?CheckLiberty(x-1,y):StateLeft;

    ChessBoard::State StateRight = CheckState(x+1,y);
    StateRight = (StateRight==Unchecked)?CheckLiberty(x+1,y):StateRight;
    /*Four Corner*/
    if(StateUp==error&&StateRight==error)
    {
        if(StateLeft==await&&StateDown==await)
        {
            PointState[x][y]=await;
            return await;
        }
        if(StateLeft==Free||StateDown==Free||(StateLeft==Black&&StateDown==White)||(StateLeft==White&&StateDown==Black))
        {
            PointState[x][y]=Free;
            return Free;
        }
        if(StateLeft==StateDown)
        {
            PointState[x][y]=StateLeft;
            return StateLeft;
        }
        if(StateLeft==Black||StateDown==Black)
        {
            PointState[x][y]=Black;
            return Black;
        }
        if(StateLeft==White||StateDown==White)
        {
            PointState[x][y]=White;
            return White;
        }
    }
    if(StateUp==error&&StateLeft==error)
    {
        if(StateRight==await&&StateDown==await)
        {
            PointState[x][y]=await;
            return await;
        }
        if(StateRight==Free||StateDown==Free||(StateRight==Black&&StateDown==White)||(StateRight==White&&StateDown==Black))
        {
            PointState[x][y]=Free;
            return Free;
        }
        if(StateRight==StateDown)
        {
            PointState[x][y]=StateRight;
            return StateRight;
        }
        if(StateRight==Black||StateDown==Black)
        {
            PointState[x][y]=Black;
            return Black;
        }
        if(StateRight==White||StateDown==White)
        {
            PointState[x][y]=White;
            return White;
        }
    }
    if(StateDown==error&&StateRight==error)
    {
        if(StateLeft==await&&StateUp==await)
        {
            PointState[x][y]=await;
            return await;
        }
        if(StateLeft==Free||StateUp==Free||(StateLeft==Black&&StateUp==White)||(StateLeft==White&&StateUp==Black))
        {
            PointState[x][y]=Free;
            return Free;
        }
        if(StateLeft==StateUp)
        {
            PointState[x][y]=StateLeft;
            return StateLeft;
        }
        if(StateLeft==Black||StateUp==Black)
        {
            PointState[x][y]=Black;
            return Black;
        }
        if(StateLeft==White||StateUp==White)
        {
            PointState[x][y]=White;
            return White;
        }
    }
    if(StateDown==error&&StateLeft==error)
    {
        if(StateRight==await&&StateUp==await)
        {
            PointState[x][y]=await;
            return await;
        }
        if(StateRight==Free||StateUp==Free||(StateRight==Black&&StateUp==White)||(StateRight==White&&StateUp==Black))
        {
            PointState[x][y]=Free;
            return Free;
        }
        if(StateRight==StateUp)
        {
            PointState[x][y]=StateRight;
            return StateRight;
        }
        if(StateRight==Black||StateUp==Black)
        {
            PointState[x][y]=Black;
            return Black;
        }
        if(StateRight==White||StateUp==White)
        {
            PointState[x][y]=White;
            return White;
        }
    }
    /*Four Edge*/
    if(StateUp==error)
    {
        if(StateDown==await&&StateLeft==await&&StateRight==await)
        {
            PointState[x][y]=await;
            return await;
        }
        if(StateDown==Free||StateLeft==Free||StateRight==Free)
        {
            PointState[x][y]=Free;
            return Free;
        }
        if(StateRight==StateDown&&StateRight==StateLeft)
        {
            PointState[x][y]=StateRight;
            return StateRight;
        }
        if(StateRight==Black||StateDown==Black||StateLeft==Black)
        {
            PointState[x][y]=Black;
            return Black;
        }
        if(StateRight==White||StateDown==White||StateLeft==White)
        {
            PointState[x][y]=White;
            return White;
        }
    }

    if(StateDown==error)
    {
        if(StateUp==await&&StateLeft==await&&StateRight==await)
        {
            PointState[x][y]=await;
            return await;
        }
        if(StateUp==Free||StateLeft==Free||StateRight==Free)
        {
            PointState[x][y]=Free;
            return Free;
        }
        if(StateRight==StateUp&&StateRight==StateLeft)
        {
            PointState[x][y]=StateRight;
            return StateRight;
        }
        if(StateRight==Black||StateUp==Black||StateLeft==Black)
        {
            PointState[x][y]=Black;
            return Black;
        }
        if(StateRight==White||StateUp==White||StateLeft==White)
        {
            PointState[x][y]=White;
            return White;
        }
    }

    if(StateLeft==error)
    {
        if(StateDown==await&&StateUp==await&&StateRight==await)
        {
            PointState[x][y]=await;
            return await;
        }
        if(StateDown==Free||StateUp==Free||StateRight==Free)
        {
            PointState[x][y]=Free;
            return Free;
        }
        if(StateRight==StateDown&&StateRight==StateUp)
        {
            PointState[x][y]=StateRight;
            return StateRight;
        }
        if(StateRight==Black||StateDown==Black||StateUp==Black)
        {
            PointState[x][y]=Black;
            return Black;
        }
        if(StateRight==White||StateDown==White||StateUp==White)
        {
            PointState[x][y]=White;
            return White;
        }
    }

    if(StateRight==error)
    {
        if(StateDown==await&&StateLeft==await&&StateUp==await)
        {
            PointState[x][y]=await;
            return await;
        }
        if(StateDown==Free||StateLeft==Free||StateUp==Free)
        {
            PointState[x][y]=Free;
            return Free;
        }
        if(StateUp==StateDown&&StateUp==StateLeft)
        {
            PointState[x][y]=StateUp;
            return StateUp;
        }
        if(StateUp==Black||StateDown==Black||StateLeft==Black)
        {
            PointState[x][y]=Black;
            return Black;
        }
        if(StateUp==White||StateDown==White||StateLeft==White)
        {
            PointState[x][y]=White;
            return White;
        }
    }
    /*Middle*/
    if(StateDown==await&&StateUp==await&&StateRight==await&&StateLeft==await)
    {
        PointState[x][y]=await;
        return await;
    }
    if(StateDown==Free||StateUp==Free||StateRight==Free||StateLeft==Free)
    {
        PointState[x][y]=Free;
        return Free;
    }
    if(StateRight==StateDown&&StateDown==StateUp&&StateUp==StateLeft)
    {
        PointState[x][y]=StateRight;
        return StateRight;
    }
    if(StateRight==Black||StateDown==Black||StateUp==Black||StateLeft==Black)
    {
        PointState[x][y]=Black;
        return Black;
    }
    if(StateRight==White||StateDown==White||StateUp==White||StateLeft==White)
    {
        PointState[x][y]=White;
        return White;
    }
}
ChessBoard::State ChessBoard::CheckState(int x, int y)
{
    if(x<0||y<0||x>18||y>18)
    {
        return error;
    }
    if(BoardGride[x][y]==1)
    {
        return White;
    }
    if(BoardGride[x][y]==2)
    {
        return Black;
    }
    return PointState[x][y];
}
void ChessBoard::SetLiberty(State liberty, int x, int y)
{
        PointState[x][y]=liberty;
        if(BoardGride[x][y-1]==0&&(x<0||y<0||x>18||y>18))
        {
            SetLiberty(liberty,x,y-1);
        }
        if(BoardGride[x][y+1]==0&&(x<0||y<0||x>18||y>18))
        {
            SetLiberty(liberty,x,y+1);
        }
        if(BoardGride[x-1][y]==0&&(x<0||y<0||x>18||y>18))
        {
            SetLiberty(liberty,x-1,y);
        }
        if(BoardGride[x+1][y]==0&&(x<0||y<0||x>18||y>18))
        {
            SetLiberty(liberty,x+1,y);
        }
}
ChessBoard::State ChessBoard::Juge(int x, int y)
{
    PointState[x][y]=await;

    ChessBoard::State StateUp = CheckState(x,y-1);
    //StateUp==(StateUp==Unchecked)?CheckLiberty(x,y-1):StateUp;

    ChessBoard::State StateDown = CheckState(x,y+1);
    //StateDown==(StateDown==Unchecked)?CheckLiberty(x,y+1):StateDown;

    ChessBoard::State StateLeft = CheckState(x-1,y);
    //StateLeft=(StateLeft==Unchecked)?CheckLiberty(x-1,y):StateLeft;

    ChessBoard::State StateRight = CheckState(x+1,y);
    //StateRight = (StateRight==Unchecked)?CheckLiberty(x+1,y):StateRight;
    /*Four Corner*/
    if(StateUp==error&&StateRight==error)
    {
        if(BoardGride[x][y]==1&&StateDown==Black&&StateLeft==Black)
        {
            BoardGride[x][y]=0;
            PointState[x][y]=Free;
            return Free;
        }
        if(BoardGride[x][y]==2&&StateDown==White&&StateLeft==White)
        {
            BoardGride[x][y]=0;
            PointState[x][y]=Free;
            return Free;
        }
    }
    if(StateDown==error&&StateRight==error)
    {
        if(BoardGride[x][y]==1&&StateUp==Black&&StateLeft==Black)
        {
            BoardGride[x][y]=0;
            PointState[x][y]=Free;
            return Free;
        }
        if(BoardGride[x][y]==2&&StateUp==White&&StateLeft==White)
        {
            BoardGride[x][y]=0;
            PointState[x][y]=Free;
            return Free;
        }
    }
    if(StateUp==error&&StateLeft==error)
    {
        if(BoardGride[x][y]==1&&StateDown==Black&&StateRight==Black)
        {
            BoardGride[x][y]=0;
            PointState[x][y]=Free;
            return Free;
        }
        if(BoardGride[x][y]==2&&StateDown==White&&StateRight==White)
        {
            BoardGride[x][y]=0;
            PointState[x][y]=Free;
            return Free;
        }
    }
    if(StateDown==error&&StateLeft==error)
    {
        if(BoardGride[x][y]==1&&StateUp==Black&&StateRight==Black)
        {
            BoardGride[x][y]=0;
            PointState[x][y]=Free;
            return Free;
        }
        if(BoardGride[x][y]==2&&StateUp==White&&StateRight==White)
        {
            BoardGride[x][y]=0;
            PointState[x][y]=Free;
            return Free;
        }
    }
    /*Edge*/
    if(StateUp==error)
    {
        if(BoardGride[x][y]==1&&StateDown==Black&&StateRight==Black&&StateLeft==Black)
        {
            BoardGride[x][y]=0;
            return Free;
        }
        if(BoardGride[x][y]==2&&StateDown==White&&StateRight==White&&StateLeft==White)
        {
            BoardGride[x][y]=0;
            return Free;
        }
    }
    if(StateDown==error)
    {
        if(BoardGride[x][y]==1&&StateUp==Black&&StateRight==Black&&StateLeft==Black)
        {
            BoardGride[x][y]=0;
            return Free;
        }
        if(BoardGride[x][y]==2&&StateUp==White&&StateRight==White&&StateLeft==White)
        {
            BoardGride[x][y]=0;
            return Free;
        }
    }
    if(StateRight==error)
    {
        if(BoardGride[x][y]==1&&StateDown==Black&&StateUp==Black&&StateLeft==Black)
        {
            BoardGride[x][y]=0;
            return Free;
        }
        if(BoardGride[x][y]==2&&StateDown==White&&StateUp==White&&StateLeft==White)
        {
            BoardGride[x][y]=0;
            return Free;
        }
    }
    if(StateLeft==error)
    {
        if(BoardGride[x][y]==1&&StateDown==Black&&StateRight==Black&&StateUp==Black)
        {
            BoardGride[x][y]=0;
            return Free;
        }
        if(BoardGride[x][y]==2&&StateDown==White&&StateRight==White&&StateUp==White)
        {
            BoardGride[x][y]=0;
            return Free;
        }
    }
    /*Middle*/

    if(BoardGride[x][y]==1&&StateDown==Black&&StateRight==Black&&StateUp==Black&&StateLeft==Black)
    {
        BoardGride[x][y]=0;
        return Free;
    }
    if(BoardGride[x][y]==2&&StateDown==White&&StateRight==White&&StateUp==White&&StateLeft==White)
    {
        BoardGride[x][y]=0;
        return Free;
    }
}
