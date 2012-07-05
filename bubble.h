#ifndef BUBBLE_H
#define BUBBLE_H

#include <QBrush>
#include <QColor>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QObject>
#include "holder.h"
#include "block.h"

#define WaitTimes 5
#define CustomSpeed 3.5
QT_BEGIN_NAMESPACE
class QPainter;
QT_END_NAMESPACE

enum WeaponType
{
    StandartWearon = 0,
    Light = 1,
    BlackHole = 2
};


class Bubble: public QObject
{
    Q_OBJECT
public:
    //Конструктор
    Bubble(const QPointF &position, qreal radius, const QPointF &velocity,const bool side);//конструктор без игрока
    Bubble(const QPointF &position, qreal radius, const QPointF &velocity,const bool side,bool Player);//описывает игрока
    //Отрисовка пузыря
    void drawBubble(QPainter *painter);//рисуем пузырек
    //Кисть
    void updateBrush();//установка текстуры
    //радиус
    QPointF getVel()
    {//получает значение скорости
        return vel;
    }

    qreal GetRad()
    {//значения радиуса
        return radius;
    }
    QPointF GetPosit()
    {//центр пузыря
        return position;
    }
    bool getSide()
    {//сторона (ситхи, елки палки)
        return Side;
    }
    void BarForward()
    {//определяет было ли столкновение
        isBarForward = true;
        PreBarForwardVel = vel;
    }

    //Движение
    //TODO (добавить проверку по сигналу PickUp), Врезание в стенки. - выполненео!
    void move(const QRect &bbox,const QList<Block*>& bars);
    //Область
    QRectF rect();
    //Установка скорости
        //по Горизонтали
    void SetVelX(int direction);
        //по вертикали
    void SetVelY(int direction);
    //Установка оружия
    void SetGun(WeaponType);

    void SaveSpeed()
    {
        if (vel.x()+vel.y())
        {
        ShiftSpeed = vel;
        PreVel = vel;
        }
    }

    void QShot(const QRect &bbox);
    //Логика ПК
    void MakeAIStep(const QList<Bubble*>& Bubbles,const QList<Block*>& Bars,int Diff,const int&Current,Bubble*&Player,const QRect &bbox);
private:
    bool isBarForward;
    int reCharge;
    bool Player;
    //сторона
    bool Side;
    //Цвет
    QColor randomColor();
    //Тип оружия
    WeaponType CurrentGun;
    //Кисть
    QBrush brush;
    QBrush GunBrush;
    //Позиция
    QPointF position;
    //Скорость
    QPointF vel;
    QPointF PreVel;
    QPointF ShiftSpeed;
    QPointF PreBarForwardVel;
    //Радиус
    qreal radius;
    //Внешний
    QColor innerColor;
    //внутр
    QColor outerColor;

public slots:
    void SetNewGun(WeaponType a){CurrentGun = a;
        if (a == StandartWearon)
        GunBrush = QBrush(QColor(255,0,0,100));
    if (a == Light)
        GunBrush = QBrush(QColor(0,0,255,100));
    if (a == BlackHole)
        GunBrush = QBrush(QColor(0,0,0,100));}//Реализовать позже
signals:

    //Поднять
    void PickUp(QPointF Position,qreal Read);//позже - TODO - удалить
    //Выстрел
   void Shot(QPointF pos,qreal Rad,QPointF Vel,WeaponType,bool Side);
};

#endif
