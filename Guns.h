#ifndef GUNSTONEWHOLDER_H
#define GUNSTONEWHOLDER_H

#include <QObject>
#include "bubble.h"

QT_BEGIN_NAMESPACE
class QPainter;
QT_END_NAMESPACE

class Guns : public QObject
{//����� ������� ������
    Q_OBJECT
public:
    explicit Guns(QObject *parent = 0);
    Guns(const QPointF &position,qreal radius,WeaponType type);
    qreal GetRad()
    {//��������� �������
        return radius;
    }
    QPointF GetPosit()
    {//�������
        return position;
    }
    WeaponType GetType()
    {//��� ������������� ������
        return WT;
    }

    QRectF rect();//�������������
    void draw(QPainter* painter);//����������
protected:
    QBrush brush;//����� - ��������
    //�������
    QPointF position;//�������
    qreal radius;//������
    WeaponType WT;//��� ������ (�� ������� =)
signals:

public slots:

};

#endif // GUNSTONEWHOLDER_H
