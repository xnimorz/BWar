#include "widget.h"
#include "ui_widget.h"
#include<stdio.h>
#include "statistic.h"
#include<QDir>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->Add->setVisible(false);
    ui->player->setVisible(false);
    FilesFinder();
    on_battleType_currentIndexChanged("Командный бой");
    PlayForm = NULL;
}


Widget::~Widget()
{
    delete ui;
}

void Widget::on_colBots_dialMoved(int value)
{
    ui->numberExCol->display(value + ui->colBots_2->value());
    ui->numberEn->display(value);

}

void Widget::on_difficultBar_dialMoved(int value)
{
    ui->numberExDif->display(value*10);
}

void Widget::on_comandsD_dialMoved(int value)
{
    ui->numberEn->display(value*10);
    ui->numberY->display(101 - value*10);
}

void Widget::on_pushButton_clicked()
{
    ui->Add->setVisible(true);
    ui->player->setVisible(true);
}
void Widget::FilesFinder()
{
    QDir FFinder("","*.pdt");


    ui->plList->clear();
    foreach(QString Str,FFinder.entryList())
    {
        Str.remove(Str.length()-4,4);
        ui->plList->addItem(Str);
    }

}

void Widget::on_Add_clicked()
{
    FILE* F = fopen((ui->player->text()+".pdt").toAscii().data(),"w");
    if (!F) return;
    fprintf(F,"%i\n%i\n%i\n%i\n%i\n",0,0,0,0,0);
    fclose(F);
    ui->Add->setVisible(false);
    ui->player->setVisible(false);
    FilesFinder();
}

void Widget::on_ShowStat_clicked()
{

 FILE *F = fopen((ui->plList->currentText()+".pdt").toAscii().data(),"r");
 if (!F) return;
 Statistic *FormSt = new Statistic(F);
 FormSt->show();
 fclose(F);
}
void Widget::BattleOver()
{
    int Battles = PlayForm->GetStatBattles();
    int Vict = PlayForm->GetVictories();
    int Lose = PlayForm->GetLosesCount();
    int Kills = PlayForm->GetKillsCount();
    int Death = PlayForm->GetDeathCount();
    QString Path = ui->plList->currentText()+".pdt";

    FILE* F = fopen(Path.toAscii().data(),"w");
    if (!F) return;
    fprintf(F,"%i\n%i\n%i\n%i\n%i\n",Battles,Vict,Lose,Kills,Death);
    fclose(F);
    FilesFinder();
    delete PlayForm;
    PlayForm = NULL;
    fclose(F);
    ui->commandLinkButton->setEnabled(true);
}

void Widget::on_commandLinkButton_clicked()
{
    FILE *F = fopen((ui->plList->currentText()+".pdt").toAscii().data(),"r");
    if (!F) QMessageBox::critical(0,"User Error","Can't open user file");
    int bat, vic, los, kil, dea;
    fscanf(F,"%i\n%i\n%i\%i\%i\n",&bat,&vic,&los,&kil,&dea);
    fclose(F);
    if (ui->battleType->currentIndex()==0)
        PlayForm = new Field(ui->battleMap->currentText(),Command,ui->colBots->value()+ui->colBots_2->value(),ui->difficultBar->value(),ui->colBots_2->value(),bat,vic,los,kil,dea);
    else if(ui->battleType->currentIndex()==1)
        PlayForm = new Field(ui->battleMap->currentText(),Alive,ui->colBots->value()+ui->colBots_2->value(),ui->difficultBar->value(),ui->colBots_2->value(),bat,vic,los,kil,dea);
    else PlayForm = new Field(ui->battleMap->currentText(),Flag,ui->colBots->value()+ui->colBots_2->value(),ui->difficultBar->value(),ui->colBots_2->value(),bat,vic,los,kil,dea);
    connect(PlayForm,SIGNAL(over()),this,SLOT(BattleOver()));
    PlayForm->setMinimumHeight(700);//1000
    PlayForm->setMinimumWidth(700);
    PlayForm->setMaximumHeight(700);
    PlayForm->setMaximumWidth(700);

    PlayForm->show();
    QSettings settings("xnimor","BubblesWar");
    QPoint pos = settings.value("pos", QPoint(0, 0)).toPoint();
    PlayForm->move(pos);
    ui->commandLinkButton->setEnabled(false);

}

void Widget::on_battleType_currentIndexChanged(QString NewText)
{
    QDir *FFinder;
    if (ui->battleType->currentIndex() == 0)
        FFinder = new QDir("","*.cmd");
    else if (ui->battleType->currentIndex() == 1)
        FFinder = new QDir("","*.alv");
    else if (ui->battleType->currentIndex() == 2)
        FFinder = new QDir("","*.dst");

    ui->battleMap->clear();
    foreach(QString Str,FFinder->entryList())
    {
        //Str.remove(Str.length()-4,4);
        ui->battleMap->addItem(Str);
    }
    delete FFinder;
}

void Widget::on_colBots_valueChanged(int value)
{

}

void Widget::on_colBots_2_dialMoved(int value)
{
    ui->numberExCol->display(value + ui->colBots->value());
    ui->numberY->display(value);
}
