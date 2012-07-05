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
{//препятствия
    Q_OBJECT
public:
    explicit Block(QObject *parent = 0);
    Block(QPointF Position,int type);//конструктор
    void draw(QPainter *painter);//рисуме
    QPointF getPos()//позиция
    {
        return pos;
    }
    QPointF getSize()//размер (стандартен =)
    {
        return size;
    }
    int getType()
    {
        return Material;
    }

    //Область
    QRectF rect();
protected:
    QPointF size;//размер
    QPointF pos;//позиция (лев верз
    int Material;//материал - добавлен для дальнейшей увеличении возможностей программы

    QBrush brush;//кисть
signals:

public slots:

};

#endif // BLOCK_H
