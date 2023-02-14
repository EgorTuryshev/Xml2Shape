#ifndef IO_SHAPE_H
#define IO_SHAPE_H

#include <QObject>
#include <iostream>

class IO_Shape
{
public:
    IO_Shape();
    void WriteShape();
    static const char* typeStr(int type);
};

#endif // IO_SHAPE_H
