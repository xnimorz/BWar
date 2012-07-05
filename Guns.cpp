#include "Guns.h"

Guns::Guns(QObject *parent) :
    QObject(parent)
{

}
Guns::Guns(const QPointF &position,qreal radius,WeaponType type)
    :position(position),radius(radius),WT(type)
{
    if (WT == StandartWearon)
    {
        brush = QBrush(QColor(255,48,70,100),Qt::SolidPattern);
    }
    else
    if (WT == Light)
    {
        brush = QBrush (QColor(50,50,250,100),Qt::SolidPattern);
    }
    else
    if (WT == BlackHole)
        {
        brush = QBrush (QColor(30,30,30,100),Qt::SolidPattern);
    }
}

QRectF Guns::rect()
{
    return QRectF(position.x() - radius, position.y() - radius,
                  2 * radius, 2 * radius);
}
void Guns::draw(QPainter* painter)
{
    {
        painter->save();
        painter->translate(position.x() - radius, position.y() - radius);
        painter->setBrush(brush);
        painter->drawEllipse(0, 0, int(2*radius), int(2*radius));

        painter->restore();
    }
}
