#ifndef GEOMETRY_ATTRIBUTE_H
#define GEOMETRY_ATTRIBUTE_H

#include "shapelib/shapefil.h"
#include <QString>

class Geometry_Attribute
{
public:
    Geometry_Attribute(int type, int field, QString value);
    int GetType();
    int GetField();
    QString GetValue();
private:
    int _type;
    int _field;
    QString _value;
};

#endif // GEOMETRY_ATTRIBUTE_H
