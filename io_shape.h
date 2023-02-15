#ifndef IO_SHAPE_H
#define IO_SHAPE_H

#include "xml_parser.h"
#include <QObject>
#include <iostream>

class IO_Shape
{
public:
    IO_Shape();
    void WriteShape();
    void WriteShape(QString featureType, xml_header header, QVector<Feature> features);
    static const char* typeStr(int type);
};

#endif // IO_SHAPE_H
