#ifndef GUNSTONEWHOLDER_H
#define GUNSTONEWHOLDER_H

#include <QObject>
#include "bubble.h"

QT_BEGIN_NAMESPACE
class QPainter;
QT_END_NAMESPACE

class Guns : public QObject
{//Пункт подбора оружия
    Q_OBJECT
public:
    explicit Guns(QObject *parent = 0);
    Guns(const QPointF &position,qreal radius,WeaponType type);
    qreal GetRad()
    {//получение радиуса
        return radius;
    }
    QPointF GetPosit()
    {//позиции
        return position;
    }
    WeaponType GetType()
    {//тип возвращаемого оружия
        return WT;
    }

    QRectF rect();//прямоугольник
    void draw(QPainter* painter);//нарисовать
protected:
    QBrush brush;//кисть - текстура
    //Позиция
    QPointF position;//позиция
    qreal radius;//радиус
    WeaponType WT;//тип оружия (на скаладе =)
signals:

public slots:

};

#endif // GUNSTONEWHOLDER_H
