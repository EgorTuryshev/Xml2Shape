#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <qvector.h>
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
    Geometry(int SHPtype);
    /*Создает ShapeFile для ВСЕХ последующих объектов Geometry.*/
    /*Задается перед первым вызовом конструктора*/
    static void SetShapeFile(const char* path);
    /*Сохранение файла в файловой системе*/
    static void SaveShapeFile();
    /*Добавить новую точку (координату) в объект*/
    void PointPush(double x, double y);
    /*Добавить свойство объекта*/
    void AddAttribute(DBFFieldType type, QString field, QString value);
    /*Начать запись части объекта со следующих координат.*/
    /*Все координаты после вызова метода будут считаться частью SHPP_RING записываемого объекта*/
    void StartSubpart();
    /*Записать объект в ShapeFile, заданный с помощью SetShapeFile()*/
    void WriteToShapeFile();
    void StartHole();
    void EndHole();
private:
    static SHPHandle shp;
    static DBFHandle dbf;
    static int SHPId;
    static int SHPtype;
    static const char* SHPpath;
    static QVector<QString> DBFFields;

    static void ResetShapeFile();

    bool isHoleOnGoing = false;
    QVector<double> hole_buffer_Xs;
    QVector<double> hole_buffer_Ys;
    int currentId;
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
    void WriteToSHP(SHPHandle shp);
    void WriteToDBF(DBFHandle dbf);
};
#endif // GEOMETRY_H
