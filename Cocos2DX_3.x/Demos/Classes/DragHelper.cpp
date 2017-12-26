#include "DragHelper.h"

DragHelper* DragHelper::_instance = nullptr;
DragHelper* DragHelper::getInstance()
{
    if(!_instance)
    {
        _instance = new DragHelper();
    }

    return _instance;
}
