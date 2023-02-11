#ifndef GEOMETRY_ATTRIBUTE_H
#define GEOMETRY_ATTRIBUTE_H

#include "shapelib/shapefil.h"
#include <string>

using namespace std;

class Geometry_Attribute
{
public:
    Geometry_Attribute(int type, int field, string value);
    int GetType();
    int GetField();
    string GetValue();
private:
    int _type;
    int _field;
    string _value;
};

#endif // GEOMETRY_ATTRIBUTE_H
