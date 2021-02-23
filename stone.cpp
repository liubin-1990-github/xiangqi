#include "stone.h"

Stone::Stone()
{
    _red = 5;
}

Stone::~Stone()
{
    this->_col = -100;
    this->_row = -100;


}
