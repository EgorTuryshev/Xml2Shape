#ifndef GEOMETRY_ATTRIBUTE_H
#define GEOMETRY_ATTRIBUTE_H

#include "shapelib/shapefil.h"
#include <QString>

class Geometry_Attribute
{
public:
    Geometry_Attribute(DBFFieldType type, int field, QString value);
    DBFFieldType GetType();
    int GetField();
    QString GetValue();
private:
    DBFFieldType _type;
    int _field;
    QString _value;
};

#endif // GEOMETRY_ATTRIBUTE_H
