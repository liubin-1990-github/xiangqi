#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include "stone.h"
#include <QMouseEvent>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QFont>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    bool _bRedTurn; //1 redturn    -1 blackturn      other  ban stone
    int _selectId ;
    Stone _s[32]; //32颗棋子
    int _r;  // 半径
    QPoint center(int row,int col);//返回棋子行列对应的像素坐标
    QPoint center(int id);
    void paintEvent(QPaintEvent *);
    void drawStone(QPainter &painter, int id);
    void mouseReleaseEvent(QMouseEvent *ev);
    bool getRowCol(QPoint pt ,int &row,int &col);
    bool canmove(int moveId, int row ,int col,int killId);
    bool canmove1(int moveId, int row ,int col,int killId);
    bool canmove2(int moveId, int row ,int col,int killId);
    bool canmove3(int moveId, int row ,int col,int killId);
    bool canmove4(int moveId, int row ,int col,int killId);
    bool canmove5(int moveId, int row ,int col,int killId);
    bool canmove6(int moveId, int row ,int col,int killId);
    bool canmove7(int moveId, int row ,int col,int killId);
    int getStoneCount(int row1,int col1,int row2,int col2); //2点之间有多少棋子
    int getStoneId(int row,int col);   //点上有棋子 返回棋子值  没有返回-1

    bool repentance(); //悔棋
    QPushButton  *_Repentance;

    int isWin(int clickedId); //判断胜利条件 红方赢返回1 黑方赢 返回 -1  没有胜利条件  返回 0；

    void WinImg(); //胜利窗口
    QPushButton  *successImg_Red;
    QPushButton  *successImg_Black;
    void redWinAnimation(QPushButton *successImg);
    void blackWinAnimation(QPushButton *successImg);
    void backAnimation(QPushButton *successImg);
    bool stone_ban;



    void redInit();
    void blackInit();
    void init();




};

#endif // WIDGET_H
