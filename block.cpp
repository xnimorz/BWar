#include "block.h"

Block::Block(QObject *parent) :
    QObject(parent)
{
}
Block::Block(QPointF Position, int type)
    :pos(Position),size(35,35),Material(type)
{
    switch (Material)
    {
        case 0:brush = QBrush(QImage("block.bmp"));
            break;
        case 1: brush = QBrush(QImage("gum.bmp"));
            break;
    case 2: brush = QBrush(QImage("glass.bmp"));
        break;
    }


}
QRectF Block::rect()
{
    return QRectF(pos,size);
}
void Block::draw(QPainter *painter)
{
    painter->save();
//    painter->translate(pos.x(), pos.y());

    painter->setBrush(brush);
    painter->drawRect(pos.x(),pos.y(),size.x(),size.y());
    painter->restore();
}
