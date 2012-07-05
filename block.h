#ifndef BLOCK_H
#define BLOCK_H

#include <QBrush>
#include <QColor>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QObject>
#include <QPainter>

#define Standart 0
#define Destroing 1

class Block : public QObject
{//�����������
    Q_OBJECT
public:
    explicit Block(QObject *parent = 0);
    Block(QPointF Position,int type);//�����������
    void draw(QPainter *painter);//������
    QPointF getPos()//�������
    {
        return pos;
    }
    QPointF getSize()//������ (���������� =)
    {
        return size;
    }
    int getType()
    {
        return Material;
    }

    //�������
    QRectF rect();
protected:
    QPointF size;//������
    QPointF pos;//������� (��� ����
    int Material;//�������� - �������� ��� ���������� ���������� ������������ ���������

    QBrush brush;//�����
signals:

public slots:

};

#endif // BLOCK_H
