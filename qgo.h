#ifndef QGO_H
#define QGO_H

#include <QObject>
#include <QList>
#include <QTextStream>
#include <QFile>
#include <QIODevice>
#include <QMessageBox>
class QGo : public QObject
{
    Q_OBJECT
public:
    explicit QGo(QObject *parent = nullptr);
    int TotalMoveNum();

public Q_SLOTS:
    void NextMove();
    void PreviousMove();
    void TenNextMove();
    void TenPreviousMove();
    void LastMove();
    void FirstMove();
    void Setting(int mode);
    void InitMove();
    void SetNode(int color,int x, int y);
    int SaveSGF();
private:
    int TotalMoveCount;
    int Mode;
    enum Color{
        White,
        Black
    };
    int CurrentMoveNum;
    struct MoveInfo{
        Color color;
        int Count;
        int x;
        int y;
    };
    QList<MoveInfo> *MoveRecord;
signals:
    void valueChanged(int newValue);

};

#endif // QGO_H
