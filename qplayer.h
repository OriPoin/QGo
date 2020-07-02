#ifndef QPLAYER_H
#define QPLAYER_H

#include <QObject>
#include <QString>

class QPlayer : public QObject
{
    Q_OBJECT
public:
    explicit QPlayer(QObject *parent = nullptr);
    int MoveCountNum();
    void SetColor(int color);
    void SetName(char* name);
    void SetRank(char* rank);
    char name[64];
    char rank[64];
private:
    int MoveCount;
    int color;
    int Caps;
signals:

};

#endif // QPLAYER_H
