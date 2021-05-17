#include "netgame.h"
#include <QDebug>

netgame::netgame(bool server)
{
    qDebug()<<"dsa";
    m_sever=NULL;
    m_socket=NULL;
    if(server == true)
    {
        m_sever = new QTcpServer;
        m_sever->listen(QHostAddress::Any,9999);
        connect(m_sever,&QTcpServer::newConnection,this,&netgame::slotNewConnet);
    }
    else
    {
        m_socket = new QTcpSocket;
        m_socket->connectToHost(QHostAddress("127.0.0.1"),9999);
        connect(m_socket,&QTcpSocket::readyRead,this,&netgame::recv);
    }
}

void netgame::click(int row, int col)
{
    Widget::click(row,col);
    //发送给对方
    char buf[3];
    buf[0]=2;
    buf[1]=9-row;
    buf[2]=8-col;
    m_socket->write(buf,3);




}

void netgame::slotNewConnet()
{
    if(m_socket) return;
    m_socket = m_sever->nextPendingConnection();
    qDebug()<<"connect";
    connect(m_socket,&QTcpSocket::readyRead,this,&netgame::recv);
}

void netgame::recv()
{
    QByteArray ba= m_socket->readAll();
    int row =ba[1];
    int col =ba[2];
    Widget::click(row,col);
}
