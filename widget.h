#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <field.h>

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    void FilesFinder();
     Field *PlayForm;
private slots:
    void on_colBots_2_dialMoved(int value);
    void on_colBots_valueChanged(int value);
    void on_battleType_currentIndexChanged(QString );
    void on_commandLinkButton_clicked();
    void on_ShowStat_clicked();
    void on_Add_clicked();
    void on_pushButton_clicked();
    void on_comandsD_dialMoved(int value);
    void on_difficultBar_dialMoved(int value);
    void on_colBots_dialMoved(int value);
    void BattleOver();
};

#endif // WIDGET_H
