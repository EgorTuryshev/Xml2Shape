#include "geometry_attribute.h"

Geometry_Attribute::Geometry_Attribute(int type, int field, string value)
{
    this->_type = type;
    this->_field = field;
    this->_value = value;
}
int Geometry_Attribute::GetType()
{
    return this->_type;
}
int Geometry_Attribute::GetField()
{
    return this->_field;
}
string Geometry_Attribute::GetValue()
{
    return this->_value;
}
