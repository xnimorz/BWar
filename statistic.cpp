#include "statistic.h"
#include "ui_statistic.h"
#include <stdio.h>

Statistic::Statistic(FILE *F, QWidget *parent):
    QWidget(parent),
    ui(new Ui::Statistic)
{

    ui->setupUi(this);

    int a;
    fscanf(F,"%i",&a);

    ui->label_1->setText(QString(QString::number(a)).toAscii());
    fscanf(F,"%i",&a);
    ui->label_2->setText(QString::number(a));
    fscanf(F,"%i",&a);
    ui->label_3->setText(QString::number(a));
    fscanf(F,"%i",&a);
    ui->label_4->setText(QString::number(a));
    fscanf(F,"%i",&a);
    ui->label_5->setText(QString::number(a));


}

Statistic::~Statistic()
{
    delete ui;
}
