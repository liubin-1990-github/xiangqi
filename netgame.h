#ifndef NETGAME_H
#define NETGAME_H

#include "widget.h"
#include <QTcpServer>
#include <QTcpSocket>

class netgame :public Widget
{
    Q_OBJECT
public:
    netgame(bool server);
    void click(int row,int col);

    QTcpServer *m_sever;
    QTcpSocket *m_socket;

    void slotNewConnet();
    void recv();
};

#endif // NETGAME_H
