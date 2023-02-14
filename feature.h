#ifndef FEATURE_H
#define FEATURE_H

#include <QString>
#include <QVector>

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

struct Attribute
{
private:
    QString name;
    QString value;
public:
    Attribute(QString name, QString value)
    {
        this->name = name;
        this->value = value;
    }
    Attribute()
    {
        this->name = "";
        this->value = "";
    }
    void setName(QString name)
    {
        this->name = name;
    }
    void setValue(QString value)
    {
        this->value = value;
    }
    QString getName()
    {
        return this->name;
    }
    QString getValue()
    {
        return this->value;
    }
};

using Shells = QVector< QVector<Coordinate> >;

class Feature
{
private:
    QVector<Attribute> attributes;
    Shells shells;
public:
    Feature(QVector<Attribute> attributes, Shells shells);
    Feature();

    void setAttributes(QVector<Attribute> attributes) { this->attributes = attributes; }
    void setShells(Shells shells) { this->shells = shells; }
    QVector<Attribute> getAttributes() { return this->attributes; }
    Shells getShells() { return this->shells; }
};

#endif // FEATURE_H
