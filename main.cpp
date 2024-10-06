#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);//创建一个QApplication类的实例，负责管理应用程序的许多核心功能+


    Widget w;
    w.show();
    return a.exec();//启动程序的事件循环
}
