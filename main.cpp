#include "widget.h"
#include <QApplication>
#include "netgame.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QMessageBox::StandardButton ret;
//    ret=QMessageBox::question(NULL,"server or client","be a sever");
//    bool bServer=false;
//    if(ret ==QMessageBox::Yes)
//    {
//        bServer = true;  //���캯���Ӹ�bool���� �����true ��
//    }
    netgame w(true);
    netgame s(false);
    w.show();
    s.show();

    return a.exec();
}
