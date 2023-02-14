#ifndef GEOMETRY_H
#define GEOMETRY_H

#include<qvector.h>
#include "shapelib/shapefil.h"
#include <geometry_attribute.h>
#include <loggingcategories.h>

using namespace std;

class Geometry
{
public:
    /*ВАЖНО!*/
    /*Запись объекта происходит итеративно,*/
    /*изменение уже записанных полей не предполагается*/
    Geometry();
    /*Добавить новую точку (координату) в объект*/
    void PointPush(double x, double y);
    /*Записать весь объект в SHPHandle*/
    void WriteToSHP(SHPHandle shp);
    /*Записать весь объект в DBFHandle*/
    /*Поля должны быть предварительно созданы*/
    void WriteToDBF(DBFHandle dbf);
    /*Добавить свойство объекта*/
    void AddAttribute(int type, int field, QString value);
    /*Начать запись части (дырки) объекта со следующих координат.*/
    /*Все координаты после вызова метода будут считаться частью SHPP_RING записываемого объекта*/
    void StartHole();
    /*Закончить запись части*/
    void EndHole();

    void SetSHPtype(int SHPtype)
    {
        this->SHPtype = SHPtype;
    }
private:
    static int SHPId;
    int currentId;
    int SHPtype;
    int nParts = 1;
    int nVerts = 0;
    QVector<double> Xs;
    QVector<double> Ys;
    QVector<int> IteratorsOfVerts {0};
    QVector<Geometry_Attribute> GATS;

    void XPush(double x);
    void YPush(double y);
    QVector<double> GetXs();
    QVector<double> GetYs();
};
#endif // GEOMETRY_H
