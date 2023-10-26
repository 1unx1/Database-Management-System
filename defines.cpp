#include "defines.h"

unsigned char h[61];

void AttrVal::_setVarcharVal(const char *varchar, int len)
{
    _varcharLen = len;
    if (_varcharVal != nullptr)
        delete[] _varcharVal;
    _varcharVal = new char[len];
    memcpy(_varcharVal, varchar, len);
}

const Byte *AttrVal::_val() const
{
    if (_isNull)
        return nullptr;
    switch (_type)
    {
    case INT:
        return (const Byte *)(&_intVal);
    case FLOAT:
        return (const Byte *)(&_floatVal);
    case VARCHAR:
        return (const Byte *)(_varcharVal);
    default:
        break;
    }
    return nullptr;
}

int AttrVal::_len() const
{
    switch (_type)
    {
    case INT:
        return 4;
    case FLOAT:
        return 4;
    case VARCHAR:
        return _varcharLen;
    default:
        break;
    }
    return 0;
}
