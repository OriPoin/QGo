#include "qplayer.h"

QPlayer::QPlayer(QObject *parent) : QObject(parent)
{
    MoveCount=0;
}

int QPlayer::MoveCountNum()
{
    return MoveCount;
}
void QPlayer::SetColor(int color)
{
    this->color=color;
}
void QPlayer::SetName(char *name)
{
    qstrcpy(this->name,name);
}
void QPlayer::SetRank(char *rank)
{
    qstrcpy(this->rank,rank);
}

