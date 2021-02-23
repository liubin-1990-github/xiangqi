#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //把32个棋子画入棋盘
    stone_ban =false;
    init();
    this->WinImg();//初始化 胜利图片
    _Repentance=new QPushButton(this);


    _Repentance->setText("悔棋");
    _Repentance->move(this->width()*0.9,this->height()*0.5);
    connect(_Repentance,&QPushButton::released,this,&Widget::repentance);

    connect(successImg_Black,&QPushButton::released,this,&Widget::blackInit);
    connect(successImg_Red,&QPushButton::released,this,&Widget::redInit);




}

Widget::~Widget()
{

}

QPoint Widget::center(int row, int col)
{
    //第col列 第row行的像素坐标 从0开始
    return QPoint((col+1)*_r*2,(row+1)*_r*2);
}

QPoint Widget::center(int id)
{
    return center(_s[id]._row,_s[id]._col);
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int d=40;
    _r=d/2;
    //画10条横线
    for(int i=1;i<11;++i)
    {
        painter.drawLine(QPoint(d,i*d),QPoint(9*d,i*d));
    }
    //画九竖线
    for(int i=1;i<10;++i)
    {
        if(i==1 || i==9)
        {
          painter.drawLine(QPoint(i*d,d),QPoint(i*d,10*d));
        }
        else
        {
            painter.drawLine(QPoint(i*d,d),QPoint(i*d,5*d));
            painter.drawLine(QPoint(i*d,6*d),QPoint(i*d,10*d));
        }
    }

    //九宫格
    painter.drawLine(QPoint(4*d,d),QPoint(6*d,3*d));
    painter.drawLine(QPoint(4*d,3*d),QPoint(6*d,d));
    painter.drawLine(QPoint(4*d,8*d),QPoint(6*d,10*d));
    painter.drawLine(QPoint(4*d,10*d),QPoint(6*d,8*d));

    //绘制32颗棋子

    for(int i=0;i<32;++i)
    {
        drawStone(painter,i);
    }
}

void Widget::drawStone(QPainter &painter, int id)
{
    //棋子死了 更新后就不画了
    if(_s[id]._dead){
        return;
    }

    QPoint c = center(id);
    QRect rect=QRect(c.x()-_r,c.y()-_r,_r*2,_r*2);
    if(id==_selectId)
    {
        painter.setBrush(QBrush("gray"));
    }else{
        painter.setBrush(QBrush("gold"));
    }



    if(_s[id]._red)
    {
        painter.setPen(Qt::red);
    }
    else{
        painter.setPen(Qt::black);
    }
    painter.drawEllipse(center(id),_r,_r);
    painter.setFont(QFont("宋体",_r,QFont::Bold));

    painter.drawText(rect,_s[id].getText(),QTextOption(Qt::AlignCenter));
}

void Widget::mouseReleaseEvent(QMouseEvent *ev)
{
    if(stone_ban == true)
    {
        return;
    }
   QPoint pt = ev->pos();
   int row,col;
   //将pt转换成象棋的行列值 引入记录进 row col
   bool bRet=getRowCol(pt,row,col);
   if(bRet==false)
   {
       return ;
   }
   //判断行列值上面有没有棋子


   int i=0;
   int clickId=-1;
   for(i=0;i<32;i++)
   {
       if(_s[i]._row ==row &&_s[i]._col==col&&_s[i]._dead==false)
       {
           break;
       }
   }
   //i 棋子被选中
    if(i<32)
    {
        clickId =i;
    }
    if(_selectId==-1) //点击要么是选择棋子  else 移动棋子
    {
        if(clickId!=-1)
        {
            if(_bRedTurn ==_s[clickId]._red)
            {
               _selectId =clickId; //选中的棋子存入selectId
               update();//选中棋子变色
            }
//            else if(_bRedTurn !=_s[clickId]._red)
//            {
//                return;
//            }

        }
    }

    //_selectId != -1 的话 说明已经选择了棋子 点第二次
    else   //移动棋子
    {
        if(canmove(_selectId,row,col,clickId))
        {
        //走棋代码
        _s[_selectId]._row=row;  //选择的棋子 行列更新到第二次点击的地方
        _s[_selectId]._col =col;
        if(clickId!=-1)
        {
            _s[clickId]._dead=true; //点击的有棋子就死
            int win=isWin(clickId);

            if(win == 1)
            {
                //red win
                redWinAnimation(successImg_Red);
                stone_ban = true;
                // ban stone


            }

            if(win  == 2)
            {
                //black win
                blackWinAnimation(successImg_Black);

                // ban stone
                stone_ban = true;
            }


            _s[clickId].~Stone();

        }


        _selectId =-1;
        _bRedTurn =!_bRedTurn;
        update();  //更新第二次点击
        }
    }





}


//效率不高
bool Widget::getRowCol(QPoint pt, int &row, int &col)
{
    for(row=0;row<=9;row++)
    {
        for(col=0;col<=8;col++)
        {
            QPoint c=center(row,col);
            int dx=c.x()-pt.x();
            int dy=c.y()-pt.y();
            int dist=dx*dx+dy*dy;
            if(dist<_r*_r){
                return true;
            }

        }
    }
    return false;
}

bool Widget::canmove(int moveId, int row, int col, int killId)
{
//    if(_s[moveId]._red==_s[killId]._red)
//    {
//        _selectId =killId;
//        update();
//        return false;
//    }
    if(_bRedTurn&&killId >=0 &&killId<16)
    {
        _selectId = killId;
        update();
        return false;
    }
    if(!_bRedTurn&&killId >=16 &&killId<32)
    {
        _selectId = killId;
        update();
        return false;
    }
    switch (_s[moveId]._type) {
    case Stone::JIANG:
        return canmove1(moveId,row,col,killId);
        break;
    case Stone::SHI:
        return canmove2(moveId,row,col,killId);
        break;
    case Stone::XIANG:
        return canmove3(moveId,row,col,killId);
        break;
    case Stone::MA:
        return canmove4(moveId,row,col,killId);
        break;
    case Stone::CHE:
        return canmove5(moveId,row,col,killId);
        break;
    case Stone::PAO:
        return canmove6(moveId,row,col,killId);
        break;
    case Stone::BING:
        return canmove7(moveId,row,col,killId);
        break;

    default:
        break;
    }

    return true;

}


//将
bool Widget::canmove1(int moveId, int row, int col, int killId)
{
    /*
     *  1.位置在九宫内
     *  2.只能移动一个格子
    */
    if(_s[moveId]._red)
    {
        if(row>=3)return false;

    }
    else {
        if(row<7){return false;}
    }
    if(col<3) return false;
    if(col>5) return false;

    int dr=_s[moveId]._row-row;
    int dc=_s[moveId]._col -col;

    int d =abs(dr)*10 +abs(dc);
    if(d==1 || d==10){return true;}
    return false;
}


//士
bool Widget::canmove2(int moveId, int row, int col, int killId)
{
    if(_s[moveId]._red)
    {
        if(row>=3)return false;

    }
    else {
        if(row<7){return false;}
    }
    if(col<3) return false;
    if(col>5) return false;

    int dr=_s[moveId]._row-row;
    int dc=_s[moveId]._col -col;

    int d =abs(dr)*10 +abs(dc);
    if(d==11){return true;}
    return false;

}


//相
bool Widget::canmove3(int moveId, int row, int col, int killId)
{
    int dr=_s[moveId]._row-row;
    int dc=_s[moveId]._col -col;
    int d =abs(dr)*10 +abs(dc);
    if(d !=22){ return false;}
    int rEye = (row + _s[moveId]._row)/2;
    int cEye = (col + _s[moveId]._col)/2;
    if(getStoneId(rEye,cEye)!=-1){return false;}

    if(_s[moveId]._red)
    {
        if(row>5){return false;}
    }
    else
    {
        if(row <4){return false;}
    }
    return true;
}

//马
bool Widget::canmove4(int moveId, int row, int col, int killId)
{
    int dr=_s[moveId]._row-row;
    int dc=_s[moveId]._col -col;
    int d =abs(dr)*10 +abs(dc);
    if(d !=12 &d!=21){return false;}
    if(d==12)
    {
        if(getStoneId(_s[moveId]._row,(col+_s[moveId]._col)/2)!=-1){return false;}
    }
    else
    {
        if(getStoneId((row+_s[moveId]._row)/2,_s[moveId]._col)!=-1){return false;}
    }

    return true;
}

//车
bool Widget::canmove5(int moveId, int row, int col, int killId)
{
    int ret =getStoneCount(row,col,_s[moveId]._row,_s[moveId]._col);
    if(ret==0){return true;}
    return false;
}
//炮
bool Widget::canmove6(int moveId, int row, int col, int killId)
{
    int ret =  getStoneCount(row,col,_s[moveId]._row,_s[moveId]._col);
    if(killId!=-1)
    {
        if(ret == 1) return true;
    }
    else
    {
        if(ret == 0) return true;
    }
    return false;
}


//兵
bool Widget::canmove7(int moveId, int row, int col, int killId)
{
    int dr=_s[moveId]._row-row;
    int dc=_s[moveId]._col -col;
    int d =abs(dr)*10 +abs(dc);
    if(d!=1&&d!=10){return false;}
    if(_s[moveId]._red)
    {
        if(_s[moveId]._row>row){return false;}
        if(_s[moveId]._row<=4 && _s[moveId]._row == row){return false;}

    }
    else
    {
        if(_s[moveId]._row<row){return false;}
        if(_s[moveId]._row>=5&&_s[moveId]._row == row){return false;}
    }
    return true;

    return true;
}

int Widget::getStoneCount(int row1, int col1, int row2, int col2)
{
    int ret=0;

    if(row1!=row2 &&col1!=col2){
        return -1;
    }
    if(row1==row2 &&col1==col2){
        return -1;
    }

    if(row1==row2)
    {
        int min =col1<col2?col1:col2;
        int max =col1<col2?col2:col1;
        for(int col = min+1;col<max;++col)
        {
            if(getStoneId(row1,col)!=-1) {++ret;}
        }
    }
    else
    {
        int min =row1<row2?row1:row2;
        int max =row1<row2?row2:row1;
        for(int row = min+1;row<max;++row)
        {
            if(getStoneId(row,col1)!=-1) {++ret;}
        }
    }
    return ret;
}

int Widget::getStoneId(int row, int col)
{
    for(int i=0;i<32;i++)
    {
        if(_s[i]._row==row&&_s[i]._col==col)
        {
            return i;
        }
    }
    return -1;
}

bool Widget::repentance()
{

}

int Widget::isWin(int clickedId)
{
    if(clickedId == 4  )
    {
        return 2;
    }
    else if(clickedId == 20 )
    {
        return 1;
    }
    return 0;
}

void Widget::WinImg()
{

    successImg_Red = new QPushButton(this);

    successImg_Red->setSizeIncrement(150,150);

    successImg_Red->setText("红方赢了 点击重新开始 ");
    QFont font("Microsoft YaHei, 10, 75;");
    successImg_Red->setFont(font);

    successImg_Red->move((this->width()-successImg_Red->width())*0.5,-successImg_Red->height());


    successImg_Black=new QPushButton(this);
    successImg_Black->setSizeIncrement(150,150);

    successImg_Black->setText("黑方赢了 点击重新开始 ");
    QFont font1("Microsoft YaHei, 10, 75;");
    successImg_Black->setFont(font1);

    successImg_Black->move((this->width()-successImg_Black->width())*0.5,-successImg_Black->height());
}

void Widget::redWinAnimation(QPushButton *successImg)
{
    // 将胜利的图片移动下来
    QPropertyAnimation *animation = new QPropertyAnimation(successImg,"geometry");
    //设置时间间隔
    animation->setDuration(999);
    //设置开始位置
    animation->setStartValue(QRect(successImg->x(),successImg->y(),successImg->width(),successImg->height()));

    //结束位置
    animation->setEndValue(QRect(successImg->x(),successImg->y()+144,successImg->width(),successImg->height()));

    //曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}
void Widget::blackWinAnimation(QPushButton *successImg)
{
    // 将胜利的图片移动下来
    QPropertyAnimation *animation = new QPropertyAnimation(successImg,"geometry");
    //设置时间间隔
    animation->setDuration(999);
    //设置开始位置
    animation->setStartValue(QRect(successImg->x(),successImg->y(),successImg->width(),successImg->height()));

    //结束位置
    animation->setEndValue(QRect(successImg->x(),successImg->y()+144,successImg->width(),successImg->height()));

    //曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();

}

void Widget::backAnimation(QPushButton *successImg)
{
    QPropertyAnimation *animation = new QPropertyAnimation(successImg,"geometry");
    //设置时间间隔
    animation->setDuration(999);
    //设置开始位置
    animation->setStartValue(QRect(successImg->x(),successImg->y(),successImg->width(),successImg->height()));

    //结束位置
    animation->setEndValue(QRect(successImg->x(),successImg->y()-144,successImg->width(),successImg->height()));

    //曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}

void Widget::redInit()
{
    for(int i=0;i<32;i++)
    {
        _s[i].init(i);
    }
    _selectId=-1;
    stone_ban = false;
    _bRedTurn =true;

    update();
    backAnimation(successImg_Red);
}

void Widget::blackInit()
{
    for(int i=0;i<32;i++)
    {
        _s[i].init(i);
    }
    _selectId=-1;
    _bRedTurn =true;
    stone_ban = false;
    update();
    backAnimation(successImg_Black);
}

void Widget::init()
{

    for(int i=0;i<32;i++)
    {
        _s[i].init(i);
    }
    _selectId=-1;
    _bRedTurn =true; //1为 红色方
//    update();
}




