#ifndef HOLDER_H
#define HOLDER_H

#include <QObject>
#include "bubble.h"
#define standart 0
#define light 1

QT_BEGIN_NAMESPACE
class QPainter;
QT_END_NAMESPACE
class holder : public QObject
{
    Q_OBJECT
public:
    holder(QObject *parent = 0);
    holder(const QPointF &position, qreal radius, const QPointF &velocity,int Type,int Who);
void draw(QPainter *painter);
//Движение
int move(const QRect &bbox);
//Область

QRectF rect();
//радиус
qreal GetRad()
{
    return radius;
}
QPointF GetPosit()
{
    return position;
}
bool getSide()
{
    return Side;
}
int getType()
{
    return gun;
}
void reverseVel()
{
    vel = -vel;
}

signals:
   // int kill(QPointF& Pos,qreal Rad);//1 - убил 0 - норм - живем -1 - врезался
private:
    QBrush brush;
    //Позиция
    QPointF position;
    //Скорость
    QPointF vel;
    //Радиус
    qreal radius;
    //Цвет
    QColor ICol;
    QColor LCol,LCol1;
    //Прорисовка
  int gun;
    bool Side;

    QColor SetBullet();
    QColor SetLight1();
    QColor SetLight2();
    QColor SetLight3();
};

#endif // HOLDER_H
