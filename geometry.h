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
    /*Проверка на запись по часовой стрелке*/
    /*Использовать не рекомендуется*/
    static bool IsClockWise(QVector<double> icw_Xs, QVector<double> icw_Ys);
    /*Переключатель умной записи субчастей*/
    /*Default = false*/
    /*При true проверяет и исправляет грязные субчасти*/
    static void SetSmartReverse(const bool isEnabled);
    /*Добавить новую точку (координату) в объект*/
    void PointPush(double x, double y);
    /*Добавить свойство объекта*/
    void AddAttribute(DBFFieldType type, QString field, QString value);
    /*Начать запись части объекта со следующих координат.*/
    /*Все координаты после вызова метода будут считаться частью SHPP_RING записываемого объекта*/
    void StartSubpart();
    /*Записать объект в ShapeFile, заданный с помощью SetShapeFile()*/
    void WriteToShapeFile();
    /*Начать запись дырки*/
    void StartHole();
    /*Закончить запись дырки*/
    void EndHole();
    /*НЕ ИСПОЛЬЗОВАТЬ*/
    void Reverse();

    QVector<double> GetXs();
    QVector<double> GetYs();
private:
    static bool isSmartReverseEnabled;
    static SHPHandle shp;
    static DBFHandle dbf;
    static int SHPId;
    static int SHPtype;
    static const char* SHPpath;
    static QVector<QString> DBFFields;

    static void ResetShapeFile();

    bool isHoleOnGoing = false;
    int currentId;
    int nParts = 0;
    int nVerts = 0;
    QVector<double> Xs;
    QVector<double> Ys;
    QVector<int> IteratorsOfVerts {0};
    QVector<Geometry_Attribute> GATS;

    void XPush(double x);
    void YPush(double y);
    void WriteToSHP(SHPHandle shp);
    void WriteToDBF(DBFHandle dbf);
    void SmartReverse();
    void Reverse(QVector<double> &buffered_Xs, QVector<double> &buffered_Ys);
};
#endif // GEOMETRY_H
