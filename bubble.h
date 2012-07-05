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
    //�����������
    Bubble(const QPointF &position, qreal radius, const QPointF &velocity,const bool side);//����������� ��� ������
    Bubble(const QPointF &position, qreal radius, const QPointF &velocity,const bool side,bool Player);//��������� ������
    //��������� ������
    void drawBubble(QPainter *painter);//������ �������
    //�����
    void updateBrush();//��������� ��������
    //������
    QPointF getVel()
    {//�������� �������� ��������
        return vel;
    }

    qreal GetRad()
    {//�������� �������
        return radius;
    }
    QPointF GetPosit()
    {//����� ������
        return position;
    }
    bool getSide()
    {//������� (�����, ���� �����)
        return Side;
    }
    void BarForward()
    {//���������� ���� �� ������������
        isBarForward = true;
        PreBarForwardVel = vel;
    }

    //��������
    //TODO (�������� �������� �� ������� PickUp), �������� � ������. - ����������!
    void move(const QRect &bbox,const QList<Block*>& bars);
    //�������
    QRectF rect();
    //��������� ��������
        //�� �����������
    void SetVelX(int direction);
        //�� ���������
    void SetVelY(int direction);
    //��������� ������
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
    //������ ��
    void MakeAIStep(const QList<Bubble*>& Bubbles,const QList<Block*>& Bars,int Diff,const int&Current,Bubble*&Player,const QRect &bbox);
private:
    bool isBarForward;
    int reCharge;
    bool Player;
    //�������
    bool Side;
    //����
    QColor randomColor();
    //��� ������
    WeaponType CurrentGun;
    //�����
    QBrush brush;
    QBrush GunBrush;
    //�������
    QPointF position;
    //��������
    QPointF vel;
    QPointF PreVel;
    QPointF ShiftSpeed;
    QPointF PreBarForwardVel;
    //������
    qreal radius;
    //�������
    QColor innerColor;
    //�����
    QColor outerColor;

public slots:
    void SetNewGun(WeaponType a){CurrentGun = a;
        if (a == StandartWearon)
        GunBrush = QBrush(QColor(255,0,0,100));
    if (a == Light)
        GunBrush = QBrush(QColor(0,0,255,100));
    if (a == BlackHole)
        GunBrush = QBrush(QColor(0,0,0,100));}//����������� �����
signals:

    //�������
    void PickUp(QPointF Position,qreal Read);//����� - TODO - �������
    //�������
   void Shot(QPointF pos,qreal Rad,QPointF Vel,WeaponType,bool Side);
};

#endif
