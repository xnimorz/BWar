#include <QtGui/QApplication>
#include<QtCore/QTextCodec>
#include "widget.h"

int main(int argc, char *argv[])
{  QTextCodec::setCodecForTr(QTextCodec::codecForName("CP1251"));
    QApplication a(argc, argv);

    Widget w;
    w.show();

    return a.exec();
}
