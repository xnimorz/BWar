#include <QtGui>
#include "bubble.h"
Bubble::Bubble(const QPointF &position, qreal radius, const QPointF &velocity,const bool side,bool Player)
         : position(position), vel(velocity), radius(radius),Side(side),isBarForward(false),reCharge(0),Player(true),ShiftSpeed(0,0)
{
    innerColor = randomColor();

    outerColor = QColor(250,240,30,150);
    updateBrush();
    this->SetGun(StandartWearon);
    GunBrush = QBrush(QColor(255,50,50,100));


}

Bubble::Bubble(const QPointF &position, qreal radius, const QPointF &velocity,const bool side)
    : position(position), vel(velocity), radius(radius),Side(side),isBarForward(false),reCharge(0),Player(false)
{
    innerColor = randomColor();
    outerColor = Side?QColor(50,50,250,150):QColor(50,250,50,150);

    updateBrush();
    this->SetGun(StandartWearon);
}
void Bubble::SetGun(WeaponType Gun)
{
    CurrentGun = Gun;

    if (Gun == StandartWearon)
        GunBrush = QBrush(QColor(250,50,50,100));
    if (Gun == light)
        GunBrush = QBrush(QColor(50,50,250,100));
    if (Gun == BlackHole)
        GunBrush = QBrush(QColor(0,0,0,100));
}

void Bubble::MakeAIStep(const QList<Bubble*>& Bubbles,const QList<Block*>& Bars, int Diff,const int&Current, Bubble*&Player,const QRect &bbox)
{//√енерируем выстрелы
    if (reCharge) reCharge--; //перезар€дка AI
    if (Player->Side!= Side)
    {
    if (Player->position.x() < position.x()+radius && Player->position.x() > position.x()-radius)
    {//≈сли по X совпадает с игроком
        bool CanShoot = !reCharge;
        for (int i = 0 ; i < Bars.count() && CanShoot; i++)
        {//проверка столкновений с блоками снар€да
            if (Bars[i]->getPos().x() < position.x() && Bars[i]->getPos().x()+Bars[i]->getSize().x() > position.x() && ((position.y() > Bars[i]->getPos().y() && Player->position.y() < Bars[i]->getPos().y() )||(position.y() < Bars[i]->getPos().y() && Player->position.y() > Bars[i]->getPos().y())) )
                CanShoot = false;
        }
        if (CanShoot)
        {//если выстрел возможен - стрел€ем
            QPointF SaveVel = vel;
            SetVelY(position.y() > Player->position.y()?-1:1);
            SetVelX(0);
            QShot(bbox);
            vel = SaveVel;
            reCharge = WaitTimes;
        }
    }
    if (Player->position.y() < position.y()+radius && Player->position.y() > position.y()-radius)
    {//по Y совпадает с игроком
        bool CanShoot = !reCharge;
        for (int i = 0 ; i < Bars.count() && CanShoot; i++)
        {
            if (Bars[i]->getPos().y() < position.y() && Bars[i]->getPos().y()+Bars[i]->getSize().y() > position.y() && ((position.x() > Bars[i]->getPos().x() && Player->position.x() < Bars[i]->getPos().x() )||(position.x() < Bars[i]->getPos().x() && Player->position.x() > Bars[i]->getPos().x())) )
                CanShoot = false;
        }
        if (CanShoot)
        {
            QPointF SaveVel = vel;
            SetVelX(position.x() > Player->position.x()?-1:1);
            SetVelY(0);
            QShot(bbox);
            vel = SaveVel;
            reCharge = WaitTimes;
        }
    }
}
    for (int j = 0 ; j < Bubbles.count(); j++)
        if (Bubbles[j]->Side != Side && j != Current)
    {//проверка на м€ч - м€ч - разных команд
        if (Bubbles[j]->position.x() < position.x()+radius && Bubbles[j]->position.x() > position.x()-radius)
        {
            bool CanShoot = !reCharge;
            for (int i = 0 ; i < Bars.count() && CanShoot; i++)
            {
                if (Bars[i]->getPos().x() < position.x() && Bars[i]->getPos().x()+Bars[i]->getSize().x() > position.x() && ((position.y() > Bars[i]->getPos().y() && Player->position.y() < Bars[i]->getPos().y() )||(position.y() < Bars[i]->getPos().y() && Bubbles[j]->position.y() > Bars[i]->getPos().y())) )
                    CanShoot = false;
            }
            if (CanShoot)
            {
                QPointF SaveVel = vel;
                SetVelY(position.y() > Bubbles[j]->position.y()?-1:1);
                SetVelX(0);
                QShot(bbox);
                vel = SaveVel;
                reCharge = WaitTimes;
            }
        }
        if (Bubbles[j]->position.y() < position.y()+radius && Bubbles[j]->position.y() > position.y()-radius)
        {
            bool CanShoot = !reCharge;
            for (int i = 0 ; i < Bars.count() && CanShoot; i++)
            {
                if (Bars[i]->getPos().y() < position.y() && Bars[i]->getPos().y()+Bars[i]->getSize().y() > position.y() && ((position.x() > Bars[i]->getPos().x() && Bubbles[j]->position.x() < Bars[i]->getPos().x() )||(position.x() < Bars[i]->getPos().x() && Bubbles[j]->position.x() > Bars[i]->getPos().x())) )
                    CanShoot = false;
            }
            if (CanShoot)
            {
                QPointF SaveVel = vel;
                SetVelX(position.x() > Bubbles[j]->position.x()?-1:1);
                SetVelY(0);
                QShot(bbox);
                vel = SaveVel;
                reCharge = WaitTimes;
            }
        }
    }
    if (vel.x() == 0 && vel.y() == 0)
        Diff = 1;
//конец выстрелов. Ћќгика движени€ пузырей
    if(Diff < 5)
    {
        if (isBarForward)
        {
            Diff = 1;
            isBarForward = false;
        }
    }
    if (Diff >= 5)
    {
        if (isBarForward)
        {
            if (vel.x()>0) {SetVelY(1); SetVelX(0);}else
                if (vel.x()<0) {SetVelY(-1); SetVelX(0);} else
                    if (vel.y()>0) {SetVelX(-1);SetVelY(0);} else
                        if (vel.y()<0) {SetVelX(1);SetVelY(0);}
        }
        if (Diff >=7)
        {
            if (reCharge == WaitTimes )
                {
                    bool changePath = 1*qrand()/(RAND_MAX+1.0);
                    if (changePath || Diff == 8 || Diff == 10)
                    if (Diff >=9)
                    {
                        if (vel.x()>0) {SetVelY(1); SetVelX(0);}else
                            if (vel.x()<0) {SetVelY(-1); SetVelX(0);} else
                                if (vel.y()>0) {SetVelX(-1);SetVelY(0);} else
                                    if (vel.y()<0) {SetVelX(1);SetVelY(0);}
                    }
                    else
                        Diff = 1;

                }
        }
    }
if (Diff <= 1)
    {//Ћегка€ сложность, рандомное движение
        int Step = 4.0*qrand()/(RAND_MAX+1.0);
        switch (Step)
        {
        case 0:
            SetVelX(-1);
            SetVelY(0);
            break;
        case 1:
            SetVelX(1);
            SetVelY(0);
            break;
        case 2:
            SetVelX(0);
            SetVelY(-1);
            break;
        default:
            SetVelX(0);
            SetVelY(1);
            break;
        }
    }




}

void Bubble::updateBrush()
{
    QRadialGradient gradient(QPointF(radius, radius), radius,
                             QPointF(radius*0.5, radius*0.5));

    gradient.setColorAt(0, QColor(255, 255, 255, 255));
    gradient.setColorAt(0.25, innerColor);
    gradient.setColorAt(1, outerColor);
    brush = QBrush(gradient);
}

void Bubble::drawBubble(QPainter *painter)
{
    painter->save();
    painter->translate(position.x() - radius, position.y() - radius);
    painter->setBrush(brush);
    painter->drawEllipse(0, 0, int(2*radius), int(2*radius));
    if (Player)
    {
        painter->setBrush(GunBrush);
        if (vel.x()+vel.y())
        {
           if (vel.x()>0)
               painter->drawEllipse(radius+radius-8,radius-4,8,8);
           if (vel.x()<0)
               painter->drawEllipse(0,radius-4,8,8);
           if (vel.y()>0)
               painter->drawEllipse(radius-4,radius+radius-8,8,8);
           if (vel.y() < 0)
               painter->drawEllipse(radius-4,0,8,8);
        }
        else
         if (PreVel.x()+PreVel.y())
        {
            if (PreVel.x()>0)
                painter->drawEllipse(radius+radius-8,radius-4,8,8);
            if (PreVel.x()<0)
                painter->drawEllipse(0,radius-4,8,8);
            if (PreVel.y()>0)
                painter->drawEllipse(radius-4,radius+radius-8,8,8);
            if (PreVel.y() < 0)
                painter->drawEllipse(radius-4,0,8,8);

        }
        else
        {
            if (ShiftSpeed.x()>0)
                painter->drawEllipse(radius+radius-8,radius-4,8,8);
            if (ShiftSpeed.x()<0)
                painter->drawEllipse(0,radius-4,8,8);
            if (ShiftSpeed.y()>0)
                painter->drawEllipse(radius-4,radius+radius-8,8,8);
            if (ShiftSpeed.y() < 0)
                painter->drawEllipse(radius-4,0,8,8);

        }

    }
    painter->restore();
}

QColor Bubble::randomColor()
{
    int red = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
    int green = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
    int blue = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
   int alpha = int(91 + 100.0*qrand()/(RAND_MAX+1.0));

    return QColor(red, green, blue, alpha);
}

void Bubble::move(const QRect &bbox,const QList<Block*>& bars)
{
    position += vel;
    qreal leftOverflow = position.x() - radius - bbox.left();
    qreal rightOverflow = position.x() + radius - bbox.right();
    qreal topOverflow = position.y() - radius - bbox.top();
    qreal bottomOverflow = position.y() + radius - bbox.bottom();

    if (leftOverflow < 0.0) {
        position-=vel;
        BarForward();
        //position.setX(position.x() - 2 * leftOverflow);
        vel.setX(0);
    } else if (rightOverflow > 0.0) {
        position-=vel;
        BarForward();
        //position.setX(position.x() - 2 * rightOverflow);
        vel.setX(0);
    }

    if (topOverflow < 0.0) {
        position-=vel;
        BarForward();
        //position.setY(position.y() - 2 * topOverflow);
        vel.setY(0);
    } else if (bottomOverflow > 0.0) {
        position-=vel;
        BarForward();
        //position.setY(position.y() - 2 * bottomOverflow);
        vel.setY(0);
    }

    foreach(Block* bar,bars)
    {
        if (bar->getType() == 2) continue;
        QPointF LeftTop = bar->getPos();
        if (sqrt(pow(position.x() - LeftTop.x(),2) + pow(position.y() - LeftTop.y(),2)) < radius )
        {
            BarForward();
            position -= vel;
            PreVel = vel;
            vel = QPointF(0,0);
        }else
        if (sqrt(pow(position.x() - (LeftTop.x()+bar->getSize().x()),2) + pow(position.y() - LeftTop.y(),2)) < radius )
        {
            BarForward();
            position -= vel;
            PreVel = vel;
            vel = QPointF(0,0);
        }else
        if (sqrt(pow(position.x() - (LeftTop.x()+bar->getSize().x()),2) + pow(position.y() - (LeftTop.y()+bar->getSize().y()),2)) < radius )
        {
            BarForward();
             position -= vel;
             PreVel = vel;
             vel = QPointF(0,0);
        }else
        if (sqrt(pow(position.x() - (LeftTop.x()),2) + pow(position.y() - (LeftTop.y()+bar->getSize().y()),2)) < radius )
        {
            BarForward();
                position -= vel;
                PreVel = vel;
                vel = QPointF(0,0);
        }else
        if (position.x()+radius > LeftTop.x() && position.x()+radius < LeftTop.x()+ bar->getSize().x() && position.y() >= LeftTop.y() && position.y() <= LeftTop.y() + bar->getSize().y())
         {
            BarForward();
            position -= vel;
            PreVel = vel;
            vel = QPointF(0,0);
        }else
        if (position.x()-radius > LeftTop.x() && position.x()-radius < LeftTop.x()+bar->getSize().x() && position.y() >= LeftTop.y() && position.y() <= LeftTop.y() + bar->getSize().y())
             {
            BarForward();
                position -= vel;
                PreVel = vel;
                vel = QPointF(0,0);
            }else
        if (position.y()+radius > LeftTop.y() && position.y()+radius < LeftTop.y()+bar->getSize().y() && position.x() >= LeftTop.x() && position.x() <= LeftTop.x() + bar->getSize().x())
                 {
                BarForward();
                    position -= vel;
                    PreVel = vel;
                    vel = QPointF(0,0);
                }else
                    if (position.y()-radius > LeftTop.y() && position.y()-radius < LeftTop.y()+bar->getSize().y() && position.x() >= LeftTop.x() && position.x() <= LeftTop.x() + bar->getSize().x())
                         {
                            BarForward();
                            position -= vel;
                            PreVel = vel;
                            vel = QPointF(0,0);
                        }
    }


}

void Bubble::QShot(const QRect &bbox)
{
    QPointF Dir = (vel.x()+vel.y())?vel:PreVel;
    if (!(Dir.x()+Dir.y()))
    {
        int res = 4*qrand()/(RAND_MAX+1.0);
        switch (res)
        {
        case 0: Dir.setX(-CustomSpeed);
            break;
        case 1: Dir.setX(CustomSpeed);
            break;
        case 2: Dir.setY(-CustomSpeed);
            break;
        case 3: Dir.setY(CustomSpeed);
            break;
        default: Dir.setX(CustomSpeed);
        }
    }
    Dir.setX(Dir.x()*2);
    Dir.setY(Dir.y()*2);
    QPointF pos;
    if (Dir.x()>0) pos.setX(position.x() + radius + CustomSpeed + 5);
        else
         if (Dir.x()<0) pos.setX(position.x() - radius - CustomSpeed - 5);
             else  pos.setX(position.x());
    if (Dir.y()>0) pos.setY(position.y() + radius + CustomSpeed + 5);
        else
            if (Dir.y()<0) pos.setY(position.y() - radius - CustomSpeed - 5);
        else pos.setY(position.y());
    qreal leftOverflow = pos.x() - bbox.left();
    qreal rightOverflow = pos.x() - bbox.right();
    qreal topOverflow = pos.y() - bbox.top();
    qreal bottomOverflow = pos.y() - bbox.bottom();
    if (leftOverflow>0 && rightOverflow <0 && topOverflow >0 && bottomOverflow <0)
        Shot(pos,6,Dir,CurrentGun,Side);
}

void Bubble::SetVelX(int direction)
{

    QPointF Temp = vel;
    vel.setX(CustomSpeed*direction);
    if (direction)
        PreVel = Temp;
}
void Bubble::SetVelY(int direction)
{
    QPointF Temp = vel;
    vel.setY(CustomSpeed*direction);
    if (direction)
        PreVel = Temp;
}

QRectF Bubble::rect()
{
    return QRectF(position.x() - radius, position.y() - radius,
                  2 * radius, 2 * radius);
}
