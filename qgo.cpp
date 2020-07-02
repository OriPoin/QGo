#include "qgo.h"

QGo::QGo(QObject *parent) : QObject(parent)
{
    TotalMoveCount=0;
    CurrentMoveNum=0;
    MoveRecord = new QList<MoveInfo>;
    MoveInfo ZeroState;
    ZeroState.Count=0;
    MoveRecord->append(ZeroState);
}
int QGo::TotalMoveNum()
{
    return TotalMoveCount;
}
void QGo::NextMove()
{
    if(TotalMoveCount==CurrentMoveNum)
    {
        TotalMoveCount++;
        CurrentMoveNum++;
    }
    if(TotalMoveCount>CurrentMoveNum)
    {
        CurrentMoveNum++;
    }
    if(TotalMoveCount<CurrentMoveNum)
    {
        CurrentMoveNum=TotalMoveCount;
    }
    //emit valueChanged(TotalMoveCount);
}
void QGo::PreviousMove()
{
    TotalMoveCount--;
    if(TotalMoveCount<0)
    {
        TotalMoveCount=0;
    }
    //emit valueChanged(TotalMoveCount);
}
void QGo::TenNextMove()
{
    TotalMoveCount+=10;
    emit valueChanged(TotalMoveCount);
}
void QGo::TenPreviousMove()
{
    TotalMoveCount-=10;
    if(TotalMoveCount<0)
    {
        TotalMoveCount=0;
    }
    //emit valueChanged(TotalMoveCount);
}
void QGo::LastMove()
{

    //emit valueChanged(TotalMoveCount);
}
void QGo::FirstMove()
{
    TotalMoveCount=0;
    //emit valueChanged(TotalMoveCount);
}

void QGo::Setting(int mode)
{
    this->Mode=mode;
}
void QGo::InitMove()
{
    TotalMoveCount=0;
}

void QGo::SetNode(int color,int x, int y)
{
    MoveInfo CurrentNode;
    if(MoveRecord->size()==TotalMoveCount)
    {
        if(color==1)
        {
            CurrentNode.color=White;
        }
        else
        {
            CurrentNode.color=Black;
        }
        CurrentNode.x=x;
        CurrentNode.y=y;
        MoveRecord->append(CurrentNode);
    }
}
int QGo::SaveSGF()
{

    QFile file("demo.sgt");

    if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) {
    QMessageBox::critical(NULL, "提示", "无法创建文件");
        return -1;
    }
    QTextStream out(&file);
    int outputcount=0;
    char x;
    char y;
    while(outputcount<TotalMoveCount)
    {
        outputcount++;
        if(MoveRecord->at(outputcount).Count==0)
        {
            out<<"(";
        }
        if(MoveRecord->at(outputcount).Count==TotalMoveCount)
        {
            out<<")";
        }
        if(MoveRecord->at(outputcount).Count>0&&MoveRecord->at(outputcount).Count<=TotalMoveCount)
        {
            out<<";";
            if(MoveRecord->at(outputcount).color==Black)
            {
                out<<"B[";
                x=MoveRecord->at(outputcount).x+97;
                y=MoveRecord->at(outputcount).y+97;
                out<<"]"<<endl;
            }
            if(MoveRecord->at(outputcount).color==White)
            {
                out<<"W[";
                x=MoveRecord->at(outputcount).x+97;
                y=MoveRecord->at(outputcount).y+97;
                out<<"]"<<endl;
            }
        }
    }
    out.flush();
    file.close();
    return 0;
}
