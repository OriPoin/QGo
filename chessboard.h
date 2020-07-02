#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QPainter>
#include <QRect>
#include <QMouseEvent>
#include <QPoint>
#include "qgo.h"

class ChessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit ChessBoard(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);
    void Place(int color,int x, int y);

    enum State
    {
        Unchecked,
        White,
        Black,
        error,
        Free,
        await,
        WhiteLiberty,
        BlackLiberty,
        FreeLiberty
    };
    State CheckLiberty(int x,int y);
    void SetLiberty(State liberty,int x,int y);
    State CheckState(int x,int y);
    State Juge(int x,int y);
public Q_SLOTS:
    void SetColor(int color);
    void StateLoop();
    void Refresh();
private:
    int BoardGride[19][19];
    int BoardPreview[19][19];
    State PointState[19][19];
    int BlackScore;
    int WhiteScore;
    int color;
    int tempx;
    int tempy;
signals:
    void Placed(int color,int x, int y);

};

#endif // CHESSBOARD_H
