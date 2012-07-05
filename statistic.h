#ifndef STATISTIC_H
#define STATISTIC_H

#include <QWidget>

namespace Ui {
    class Statistic;
}

class Statistic : public QWidget
{
    Q_OBJECT

public:
    explicit Statistic(FILE *F,QWidget *parent = 0);
    ~Statistic();

private:
    Ui::Statistic *ui;
};

#endif // STATISTIC_H
