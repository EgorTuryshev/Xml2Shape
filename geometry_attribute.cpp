#include "geometry_attribute.h"

Geometry_Attribute::Geometry_Attribute(DBFFieldType type, int field, QString value)
{
    this->_type = type;
    this->_field = field;
    this->_value = value;
}
DBFFieldType Geometry_Attribute::GetType()
{
    return this->_type;
}
int Geometry_Attribute::GetField()
{
    return this->_field;
}
QString Geometry_Attribute::GetValue()
{
    return this->_value;
}
