#ifndef FIELD_H
#define FIELD_H

#include <QBrush>
#include <QFont>
#include <QImage>
#include <QPen>
#include <QGLWidget>
#include <QTimer>
#include <QtGui>
#include <QtOpenGL>
#include <math.h>
#include "bubble.h"
#include <QObject>
#include <Guns.h>
#include "holder.h"
#include "block.h"


class Bubble;

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QWidget;
QT_END_NAMESPACE

enum MapType
{
    Not=0,
    Command = 1,
    Alive = 2,
    Flag = 3
};

class Field : public QGLWidget
{
    Q_OBJECT
public:
    Field(QString Path,MapType Type, int Bots,int Diff,int Yours,int,int,int,int,int);
    Field();
    ~Field();
    int GetStatBattles()
    {//вывод кол-ва сражений
        return BattlesCount;
    }
    int GetVictories()
    {//Возвращает кол-во побед
        return VictoriesCount;
    }
    int GetLosesCount()
    {//Возвращает кол-во поражений
        return LosesBattlesCount;
    }
    int GetKillsCount()
    {//уничтоженных
        return BubblesKill;
    }
    int GetDeathCount()
    {//потерянных
        return BubbleDeath;
    }

protected:

    void Destruct();
    void initializeGL();
    void resizeGL(int width, int height);
    void paintEvent(QPaintEvent *event);
    void setupViewport(int width, int height);
    QSize *WindowSize;
    QTimer Animate;
    QColor qtGreen,qtPurple;
    void keyPressEvent(QKeyEvent *);
    int spaceBreaker;
    bool checkKillsAndDestroy();
    void DrawBack(QPainter*);
    QBrush brush;
    Bubble* Player;

    MapType CurrentMapType;

    char** Map;
    int AIsCount,Difficult,EnemyAIs;
    int WaysAlive;

    QList<Guns*> PickedUpWeapon;
    QList<Bubble*> AI;
    QList<holder*> Bullets;
    QList<Block*> Blocks;
//Статистика
    int BattlesCount,VictoriesCount, LosesBattlesCount, BubblesKill,BubbleDeath;
 void closeEvent(QCloseEvent *)
 {
     close();
 }

signals:
    void over();
private slots:
    void animate();
    void Shot(QPointF pos,qreal Rad,QPointF Vel,WeaponType,bool);
    void close();

   // void Kill(QPointF&,qreal);
    //QList<> - Оружий, препятствий, игроков
};

#endif // FIELD_H
