#ifndef FEATURE_H
#define FEATURE_H

#include <QString>
#include <QVector>

struct Attribute
{
private:
    QString _name;
    QString _value;
public:
    Attribute(QString name, QString value)
    {
        this->_name = name;
        this->_value = value;
    }
    Attribute()
    {
        this->_name = "";
        this->_value = "";
    }
    void setName(QString name)
    {
        this->_name = name;
    }
    void setValue(QString value)
    {
        this->_value = value;
    }
    QString getName()
    {
        return this->_name;
    }
    QString getValue()
    {
        return this->_value;
    }
};

struct Coordinate
{
private:
    double x;
    double y;
    unsigned int ord_number;
public:
    Coordinate(double x, double y, unsigned int ord_number)
    {
        this->x = x;
        this->y = y;
        this->ord_number = ord_number;
    }
    Coordinate(double x, double y)
    {
        this->x = x;
        this->y = y;
        this->ord_number = 0;
    }
    Coordinate()
    {
        this->x = 0;
        this->y = 0;
        this->ord_number = 0;
    }
    void setX(double x)
    {
        this->x = x;
    }
    void setY(double y)
    {
        this->y = y;
    }
    void setOrdNumber(unsigned int ord_number)
    {
        this->ord_number = ord_number;
    }
    double getX()
    {
        return x;
    }
    double getY()
    {
        return y;
    }
    unsigned int getOrdNumber()
    {
        return ord_number;
    }
};

using GeometryObject = QVector<Coordinate>;

struct Shell
{
private:
    GeometryObject coordinates;
    QVector<GeometryObject> holes;
public:
    Shell() { this->coordinates = GeometryObject(); this->holes = QVector<GeometryObject>(); };
    Shell(GeometryObject coordinates, QVector<GeometryObject> holes) { this->coordinates = coordinates; this->holes = holes; };
    void setCoordinates(GeometryObject coordinates) { this->coordinates = coordinates; };
    GeometryObject getCoordinates() { return this->coordinates; }
    void setHoles(QVector<GeometryObject> holes) { this->holes = holes; };
    QVector<GeometryObject> getHoles() { return this->holes; }
};

class Feature
{
private:
    QVector<Attribute> attributes;
    QVector<Shell> shells;
public:
    Feature(QVector<Attribute> attributes, QVector<Shell> shells);
    Feature();

    void setAttributes(QVector<Attribute> attributes) { this->attributes = attributes; }
    void setShells(QVector<Shell> shells) { this->shells = shells; }
    QVector<Attribute> getAttributes() { return this->attributes; }
    QVector<Shell> getShells() { return this->shells; }
};

#endif // FEATURE_H
