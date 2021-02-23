#ifndef STONE_H
#define STONE_H

#include <QString>

class Stone
{
public:
    Stone();
    ~Stone();

    enum TYPE{JIANG,CHE,PAO,MA,BING,SHI,XIANG};
    int _row; //第几行
    int _col;//第几列
    int _id;
    bool _dead; //死了没
    bool _red; //颜色
    TYPE _type;//类型

    void init(int id)
    {
        struct{
            int row,col;
            Stone::TYPE type;
        }
        pos[16]={
        {0,0,Stone::CHE},
        {0,1,Stone::MA},
        {0,2,Stone::XIANG},
        {0,3,Stone::SHI},
        {0,4,Stone::JIANG},
        {0,5,Stone::SHI},
        {0,6,Stone::XIANG},
        {0,7,Stone::MA},
        {0,8,Stone::CHE},

        {2,1,Stone::PAO},
        {2,7,Stone::PAO},
        {3,0,Stone::BING},
        {3,2,Stone::BING},
        {3,4,Stone::BING},
        {3,6,Stone::BING},
        {3,8,Stone::BING},

        };
        _id=id;
        _dead = false;
        _red= id<16;
        if(id<16)
        {
            _row = pos[id].row;
            _col =pos[id].col;
            _type =pos[id].type;
        }
        else
        {
            _row = 9-pos[id-16].row; //对称的2个棋子行相加=9
            _col = 8-pos[id-16].col; //对称的2个棋子列相加=9
            _type = pos[id-16].type;
        }
    }

    QString getText()
    {
        switch (this->_type)
        {
        case CHE:
            return "车";
            break;
        case MA:
            return "马";
            break;

        case JIANG:
            return "将";
            break;
        case SHI:
            return "士";
            break;
        case XIANG:
            return "相";
            break;
        case BING:
            return "兵";
            break;
        case PAO:
            return "炮";
            break;
//        default:
//            break;
        }
        return "错误";
    }
};

#endif // STONE_H
