#include "field.h"
#include <math.h>
#include <QMessageBox>
#include <list>
#include<QCloseEvent>
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif
using namespace std;
Field::Field(QString Path, MapType Type, int Bots, int Diff, int Yours ,int battles, int VictoryCount, int LosesCount, int kills, int death)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), NULL),CurrentMapType(Type),//Настройки виджета и выбор типа карты с путем к карте
            AIsCount(Bots),EnemyAIs(Bots-Yours),Difficult(Diff),spaceBreaker(0),//Прерывание пробела
                BattlesCount(++battles),VictoriesCount(VictoryCount),LosesBattlesCount(LosesCount),BubblesKill(kills),BubbleDeath(death),WaysAlive(0)//$характеристики учетной
{
    brush = QBrush(QImage("Theme.bmp"));//тема


    int Enemy = EnemyAIs;//враги
    Map = NULL;//карта
    if (Path != "")
    {
        FILE *F = fopen(Path.toAscii().data(),"r");
        if (!F)
        {
            QMessageBox msg;
            msg.setText("Не могу грузить карту.. что-то не то.");
            msg.exec();
        }
        else
        {
            char c;
            Map = new char*[20];
            for (int i = 0 ; i < 20 ; i ++)
                Map[i] = new char[20];
            int i = 0,j = 0;
            while (c = fgetc(F),!feof(F))
            {
                if (c!='\n' && c!='\r')
                {
                    if (i==20)
                        continue;
                    Map[i][j++] = c;
                    if (j==20) i++,j=0;
                }
            }
        }

    }//Грузим карту
    Player = NULL;
    if (Map)
    {
        for (int i = 0; i < 20; i++)
            for (int j= 0; j < 20 ; j ++)
            {
                if (Map[i][j] == 'o')
                    if (!Player) Player = new Bubble(QPointF(j*35+16,i*35+16),16,QPointF(0.0,0.0),1,true);
                    else if (Yours > 0)
                        {
                            Bubble* NewAI = new Bubble(QPointF(j*35+16,i*35+16),16,QPointF(0.0,0.0),1);
                            connect(NewAI,SIGNAL(Shot(QPointF,qreal,QPointF,WeaponType,bool)),this,SLOT(Shot(QPointF,qreal,QPointF,WeaponType,bool)));
                            AI.append(NewAI);
                            Yours--;
                        }
               if (Map[i][j] == '0' && Enemy >0)
                    {
                       Bubble* NewAI = new Bubble(QPointF(j*35+16,i*35+16),16,QPointF(0.0,0.0),0);
                       connect(NewAI,SIGNAL(Shot(QPointF,qreal,QPointF,WeaponType,bool)),this,SLOT(Shot(QPointF,qreal,QPointF,WeaponType,bool)));
                       AI.append(NewAI);
                       Enemy--;
                    }
               if (Map[i][j] == '=')
               {
                   Block * NewBlock = new Block(QPointF(j*35.0,i*35),0);
                   Blocks.append(NewBlock);//стена
               }
               if (Map[i][j] == '-')
               {
                   Block * NewBlock = new Block(QPointF(j*35.0,i*35),1);
                   Blocks.append(NewBlock);//отбрасывающая стена
               }
               if (Map[i][j] == '+')
               {
                   Block * NewBlock = new Block(QPointF(j*35.0,i*35),2);
                   Blocks.append(NewBlock);//"облачная стена"
               }
               if (Map[i][j] == 's')
               {
                   Guns* newGun = new Guns(QPointF(j*35+16,i*35+16),(qreal)12,StandartWearon);
                   PickedUpWeapon.append(newGun);
               }
                if (Map[i][j] == 'l')
               {
                    Guns* newGun = new Guns(QPointF(j*35+16,i*35+16),(qreal)12,Light);
                    PickedUpWeapon.append(newGun);
                }
                if (Map[i][j] == 'b')
                {
                     Guns* newGun = new Guns(QPointF(j*35+16,i*35+16),(qreal)12,BlackHole);
                     PickedUpWeapon.append(newGun);
                 }
            }
    }
    else
    {
        Player = new Bubble(QPointF(25.0,25.0),25.0,QPointF(0.0,0.0),1);
    }
    connect(Player,SIGNAL(Shot(QPointF,qreal,QPointF,WeaponType,bool)),this,SLOT(Shot(QPointF,qreal,QPointF,WeaponType,bool)));
 //  Player->GetGun(Light);
    Animate.setSingleShot(false);
    connect(&Animate, SIGNAL(timeout()), this, SLOT(animate()));
    Animate.start(25);
    setAutoFillBackground(false);
//Определить разрешение экрана
    WindowSize = NULL;
}

Field::~Field()
{
   Destruct();

}
void Field::Destruct()
{
  //  Animate.stop();
    if (Player)
        delete Player;
    {QMutableListIterator<holder*> iter(Bullets);
    while (iter.hasNext()) {
            holder *ToDelete = iter.next();
            delete ToDelete;
        }}
    for (int i = Bullets.count() - 1; i>=0; i--)
        Bullets.removeAt(i);

    {QMutableListIterator<Bubble*> iter(AI);
    while (iter.hasNext()) {
            Bubble *ToDelete = iter.next();
            delete ToDelete;
        }}
    for (int i = AI.count() - 1; i>=0; i--)
        AI.removeAt(i);

    {QMutableListIterator<Block*> iter(Blocks);
    while (iter.hasNext()) {
            Block *ToDelete = iter.next();
            delete ToDelete;
        }}
    for (int i = Blocks.count() - 1; i>=0; i--)
        Blocks.removeAt(i);

    {QMutableListIterator<Guns*> iter(PickedUpWeapon);
    while (iter.hasNext()) {
            Guns *ToDelete = iter.next();
            delete ToDelete;
        }}
    for (int i = PickedUpWeapon.count() - 1; i>=0; i--)
        PickedUpWeapon.removeAt(i);

    delete WindowSize;
    if (Map)
    {
        for (int i = 0 ; i < 20 ; i++)
            delete[] Map[i];
        delete[] Map;
    }
}

void Field::Shot(QPointF pos, qreal Rad, QPointF Vel, WeaponType Gun,bool Side)
{
    holder* newBullet = new holder(pos,Rad,Vel,Gun,Side);
  //  connect(newBullet,SIGNAL(kill(QPointF&,qreal)),this,SLOT(Kill(QPointF&,qreal)));
    Bullets.append(newBullet);
}

/*void Kill(QPointF & Posit, qreal rad)
{
    int i = 0;
   //Реализовать попадаение
}*/
bool Field::checkKillsAndDestroy()
{
//Поднимаем оружие
        foreach(Guns* CheckGun,PickedUpWeapon)
        {//Игрок
            if (sqrt(pow(CheckGun->GetPosit().x() - Player->GetPosit().x(),2) + pow(CheckGun->GetPosit().y() - Player->GetPosit().y(),2))<=CheckGun->GetRad() + Player->GetRad() )
            {
                Player->SetNewGun(CheckGun->GetType());
            }
        }

        foreach(Guns* CheckGun,PickedUpWeapon)
        {//ПК
            for (int i = 0 ; i  < AI.count(); i++)
            if (sqrt(pow(CheckGun->GetPosit().x() - AI[i]->GetPosit().x(),2) + pow(CheckGun->GetPosit().y() - AI[i]->GetPosit().y(),2))<=CheckGun->GetRad() + AI[i]->GetRad() )
            {
                AI[i]->SetNewGun(CheckGun->GetType());
            }
        }
//столкновения пузырей
    for (int i = 0 ; i < AI.count(); i++)
    {//Столкновение игрока и ПК
        if (sqrt(pow(AI[i]->GetPosit().x() - Player->GetPosit().x(),2) + pow(AI[i]->GetPosit().y() - Player->GetPosit().y(),2))<=AI[i]->GetRad() + Player->GetRad() )
        {
            AI[i]->SetVelX(-AI[i]->getVel().x());
            AI[i]->SetVelY(-AI[i]->getVel().y());
            AI[i]->move(rect(),Blocks);
            AI[i]->SetVelX(0);
            AI[i]->SetVelY(0);
            AI[i]->BarForward();


            Player->SetVelX(-Player->getVel().x());
            Player->SetVelY(-Player->getVel().y());
          Player->move(rect(),Blocks);
           Player->SetVelX(0);
           Player->SetVelY(0);
        }
    }

    for (int i = 0; i < AI.count(); i++)
        for (int j = 0; j <AI.count(); j ++)
            if (i!=j)
        {//ПК и ПК
            if (sqrt(pow(AI[i]->GetPosit().x() - AI[j]->GetPosit().x(),2) + pow(AI[i]->GetPosit().y() - AI[j]->GetPosit().y(),2))<=AI[i]->GetRad() + AI[j]->GetRad() )
            {
                AI[i]->SetVelX(-AI[i]->getVel().x());
                AI[i]->SetVelY(-AI[i]->getVel().y());
                AI[i]->move(rect(),Blocks);
                AI[i]->SetVelX(0);
                AI[i]->SetVelY(0);
                AI[i]->BarForward();

                AI[j]->SetVelX(-AI[j]->getVel().x());
                AI[j]->SetVelY(-AI[j]->getVel().y());
                AI[j]->move(rect(),Blocks);
                AI[j]->SetVelX(0);
                AI[j]->SetVelY(0);
                AI[j]->BarForward();
            }
        }

    //Столкновение снарядов
    QList<int> ToDeleteList;
    for (int i = 0 ; i < Bullets.count(); i++)
    {//Поиск
        for (int j = 0 ; j < Bullets.count(); j++)
        {
            if (i == j ) continue;

            if (sqrt(pow(Bullets[i]->GetPosit().x() - Bullets[j]->GetPosit().x(),2) + pow(Bullets[i]->GetPosit().y() - Bullets[j]->GetPosit().y(),2))<=Bullets[i]->GetRad() + Bullets[j]->GetRad() )
            {
                 if (Bullets[i]->getType() != 2 || Bullets[i]->getType() == Bullets[j]->getType())
                ToDeleteList.append(i);
            }
        }
    }

    for (int i = ToDeleteList.count()-1 ; i >= 0; i--)
    {//удаление

            delete Bullets[ToDeleteList[i]];
        Bullets.removeAt(ToDeleteList[i]);
    }

//Попадаение снаряда в игрока (ПК)
    //Шар и снаряд
    list<int>BulletsDel,BubblesDel;
    for (int i = 0 ; i < Bullets.count(); i++)
    {//Поиск
        for (int j = 0 ; j < AI.count(); j++)
            if (sqrt(pow(Bullets[i]->GetPosit().x() - AI[j]->GetPosit().x(),2) + pow(Bullets[i]->GetPosit().y() - AI[j]->GetPosit().y(),2))<=Bullets[i]->GetRad() + AI[j]->GetRad() )
            {//ПК
                if (Bullets[i]->getSide() != AI[j]->getSide())                    //Если не союзникик, то лопнет шар
                {
                          BubblesDel.push_back(j);
                          if (AI[j]->getSide() != Player->getSide()) BubblesKill++;
                              else BubbleDeath++;
                }
                BulletsDel.push_back(i);
            }
        if (sqrt(pow(Bullets[i]->GetPosit().x() - Player->GetPosit().x(),2) + pow(Bullets[i]->GetPosit().y() - Player->GetPosit().y(),2))<=Bullets[i]->GetRad() + Player->GetRad() )
        {
            if (Bullets[i]->getSide() != Player->getSide())
            {//Снаряд и ПК
                BubbleDeath++;
                if (this->CurrentMapType != Alive)
                {
                    LosesBattlesCount++;
                    QMessageBox::critical(this,"Lose","Your Bubble is desroyed. You are lose!");
                }
                else
                {
                    QMessageBox::information(this,"over","Battle is over. You survived the " + QString::number(WaysAlive) + " ways");
                }
                close();
                return true;
            }
            BulletsDel.push_back(i);
        }
    }
    //Удаление
    BulletsDel.sort();
    BubblesDel.sort();
    for (list<int>::reverse_iterator DelNumber = BulletsDel.rbegin(); DelNumber != BulletsDel.rend(); DelNumber++)
    {
        int t = (*DelNumber);
        delete Bullets[t];
        Bullets.removeAt(t);
    }
    for (list<int>::reverse_iterator DelNumber = BubblesDel.rbegin(); DelNumber != BubblesDel.rend(); DelNumber++)
    {
        int t = (*DelNumber);
        delete AI[t];
        AI.removeAt(t);
    }
return false;
}



void Field::close()
{
    Animate.stop();
    emit over();
}

void Field::animate()
{//Анимации, проверка выживших элементов
    try
    {
    if (spaceBreaker) spaceBreaker--;//Перезарядка игрока
    Player->move(rect(),Blocks);//Движение игрока



    //Проверка на конец игры
    bool EnemyesOff = true;
    //EnemyesOff = AI.count() >0;
    int CheckIndex = 0;
    while (CheckIndex < AI.count())
    {//Сравнение с выжившими шарами
        EnemyesOff = (AI[CheckIndex]->getSide() == Player->getSide()) && EnemyesOff;
        CheckIndex++;
    }
    if (EnemyesOff)
    {
        if (CurrentMapType != Alive)
        {//победа
            VictoriesCount++;
            QMessageBox::about(this,"Victory","You are victory!!!");
            close();
            return;
        }
        else
        {//Режим игры - выживание
            int enem = EnemyAIs;
            WaysAlive++;
            for (int i = 0 ; i <20 && enem > 0; i ++)
                for (int j = 0; j < 20 && enem > 0; j++)
            {
                if (Map[i][j] == '0')
                {
                    enem--;
                    Bubble* NewAI = new Bubble(QPointF(j*35+16,i*35+16),16,QPointF(0.0,0.0),0);
                    connect(NewAI,SIGNAL(Shot(QPointF,qreal,QPointF,WeaponType,bool)),this,SLOT(Shot(QPointF,qreal,QPointF,WeaponType,bool)));
                    AI.append(NewAI);
                }
            }

        }
    }
//Ходы компьтерного ума
for (int i = 0 ; i < AI.count(); i++)
    {
        AI[i]->MakeAIStep(AI,Blocks,Difficult,i,Player,rect());
        AI[i]->move(rect(),Blocks);
    }


   QList<int> deleteList;
    for (int i = 0 ; i < Bullets.count(); i++)
    {//потери пуль от столкновения с блоками
        if (Bullets[i]->move(rect()))
        {
            deleteList.append(i);
        }
        else
        {
            bool added = false;
            for (int j = 0; j < Blocks.count() && !added; j++)
            {
                QPointF pos = Bullets[i]->GetPosit();
                QPointF blockPos = Blocks[j]->getPos();
                if (pos.x()>= blockPos.x() && pos.x() <= blockPos.x()+Blocks[j]->getSize().x() && pos.y() >= blockPos.y() && pos.y() <= blockPos.y()+Blocks[j]->getSize().y())
                {
                    if (Blocks[j]->getType() == 1)
                    {
                        Bullets[i]->reverseVel();
                    }
                    else
                        deleteList.append(i);
                    added = true;
                }
            }
        }
    }
    for (int i = deleteList.count()-1 ; i >= 0; i--)
    {
     //   if (Bullets[ToDeleteList[i]])
            delete Bullets[deleteList[i]];
        Bullets.removeAt(deleteList[i]);
    }
   if(checkKillsAndDestroy()) return;//Взимодействие с движущимися объектами



    update();//Принятие изменений
}
    catch(...)
    {
        QMessageBox::critical(0,"Error","There is an error");//Эту штуку мы не должны достичь =) была для дебага =)
    }

    //Player->SetVelX(0);
   // Player->SetVelY(0);

}


Field::Field()
    : QGLWidget(QGLFormat(QGL::SampleBuffers), NULL)
{

int i = 1/0;
      //qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
   //   qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

      Animate.setSingleShot(false);
      //connect(&Animate, SIGNAL(timeout()), this, SLOT(animate()));
      Animate.start(25);

      setAutoFillBackground(false);



    WindowSize = NULL;
}


void Field::keyPressEvent(QKeyEvent *Key)
{
    switch(Key->key())
    {
    case Qt::Key_Left:
            Player->SetVelX(-1);
            Player->SetVelY(0);
        break;
    case Qt::Key_Right:
        Player->SetVelX(1);
        Player->SetVelY(0);
        break;
    case Qt::Key_Up:
            Player->SetVelY(-1);
            Player->SetVelX(0);
        break;
    case Qt::Key_Down:
        Player->SetVelY(1);
        Player->SetVelX(0);
        break;
    case Qt::Key_Space:
        {  if (!spaceBreaker)
            {
            Player->QShot(rect());
            spaceBreaker = WaitTimes;
        }
            break;
        }
    case Qt::Key_Shift:
        {
            Player->SaveSpeed();
            Player->SetVelX(0);
            Player->SetVelY(0);
            break;
        }

    }

}


void Field::initializeGL()
{
    glEnable(GL_MULTISAMPLE);

}
void Field::resizeGL(int width, int height)
{
    setupViewport(width,height);
    if (WindowSize)
        delete WindowSize;
    WindowSize = new QSize(width,height);
}



void Field::paintEvent(QPaintEvent *event)
{
   makeCurrent();


    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();



    qglClearColor(QColor(100,150,80));

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT5);
    glEnable(GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT5, GL_POSITION, lightPosition);
    setupViewport(width(), height());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glShadeModel(GL_FLAT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();




    setupViewport(width(), height());
    QPainter painter(this);


    painter.setRenderHint(QPainter::Antialiasing);

    DrawBack(&painter);
    foreach(Guns* Gun, PickedUpWeapon)
    {
        if (Gun->rect().intersects(event->rect()))
                    Gun->draw(&painter);
    }

    foreach(Block* block, Blocks)
    {
        //if (block->rect().intersects(event->rect()))
            block->draw(&painter);
    }


    if (Player->rect().intersects(event->rect()))
        Player->drawBubble(&painter);

   foreach (Bubble *bubble, AI) {
        if (bubble->rect().intersects(event->rect()))
            bubble->drawBubble(&painter);
    }



  //drawInstructions(&painter);*/
    foreach (holder* Bullet,Bullets)
    {
        if (Bullet->rect().intersects(event->rect()))
            Bullet->draw(&painter);
    }

    painter.end();
}
void Field::DrawBack(QPainter *painter)
{
    painter->save();
//    painter->translate(pos.x(), pos.y());
    painter->setBrush(brush);
    painter->drawRect(0,0,700,700);
    painter->restore();
}

void Field::setupViewport(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES
    glOrthof(-0.5, +0.5, -0.5, 0.5, 4.0, 15.0);
#else
    glOrtho(-0.5, +0.5, -0.5, 0.5, 4.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}
