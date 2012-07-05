#include "holder.h"
#include <QPainter>
holder::holder(QObject *parent) :
    QObject(parent)
{
}
holder::holder(const QPointF &position, qreal radius, const QPointF &velocity, int Type ,int Who)
    :position(position),vel(velocity),radius(radius),gun(Type),Side(Who)
{
    if (!gun)
    {
        ICol = SetBullet();
        brush = QBrush(ICol,Qt::SolidPattern);
    }
    else
        if (gun == 1 )
    {
        vel.setX(vel.x()*2);
        vel.setY(vel.y()*2);
        ICol = SetLight1();
        LCol = SetLight2();
        LCol1 = SetLight3();
        QRadialGradient gradient(QPointF(radius, radius), radius,
                                 QPointF(radius*0.5, radius*0.5));

        gradient.setColorAt(0, ICol);
        gradient.setColorAt(0.5, LCol);
        gradient.setColorAt(1, LCol1);
        brush = QBrush(gradient);
    }
    else
    {
        vel.setX(vel.x()/2);
        vel.setY(vel.y()/2);
        if (vel.x() >0) vel.setX(vel.x() + 3);
        if (vel.x() <0) vel.setX(vel.x() - 3);
        if (vel.y() >0) vel.setY(vel.y() + 3);
        if (vel.y() <0) vel.setY(vel.y() - 3);
        ICol = QColor(0,0,0,150);
        LCol = QColor(50,50,50,150);
        QRadialGradient gradient(QPointF(radius, radius), radius,
                                 QPointF(radius*0.5, radius*0.5));
        gradient.setColorAt(0, ICol);
        gradient.setColorAt(1, LCol);
        brush = QBrush(gradient);
    }
}

void holder::draw(QPainter *painter)
{
    painter->save();
    painter->translate(position.x() - radius, position.y() - radius);
    painter->setBrush(brush);
    painter->drawEllipse(0, 0, int(2*radius), int(2*radius));
    painter->restore();
}

int holder::move(const QRect &bbox)
{

        position += vel;
        qreal leftOverflow = position.x() - radius - bbox.left();
        qreal rightOverflow = position.x() + radius - bbox.right();
        qreal topOverflow = position.y() - radius - bbox.top();
        qreal bottomOverflow = position.y() + radius - bbox.bottom();

        if (leftOverflow < 0.0) {

            return 1;
        } else if (rightOverflow > 0.0) {

            return 1;
        }

        if (topOverflow < 0.0) {

            return 1;
        } else if (bottomOverflow > 0.0) {

            return 1;
        }
    //emit kill(position,radius);
    return 0;
}

QRectF holder::rect()
{
    return QRectF(position.x() - radius, position.y() - radius,
                  2 * radius, 2 * radius);
}

QColor holder::SetBullet()
{
    return QColor(255,48,70);
}

QColor holder::SetLight1()
{
    return QColor(21,66,180);
}

QColor holder::SetLight2()
{
    return QColor(68,145,210);
}

QColor holder::SetLight3()
{
    return QColor(147,242,243);
}
