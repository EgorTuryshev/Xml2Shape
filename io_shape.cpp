#include "io_shape.h"
#include <stdlib.h>
#include <iostream>
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QDebug>
#include <QtXml>
#include <geometry.h>
#include <QDirIterator>
#include <QFileInfo>
#include <QString>
#include <fs_category.h>
#include <fs_property_manager.h>

using namespace std;

QString getUniqueFilePath(QString filePath, int i = 1) // CHECK: Я не уверен, что это оптимальный способ
{
    QFileInfo fileInfo(filePath);
    QString newFilePath = filePath;
    bool isUnique = true;

    QDirIterator it(fileInfo.dir(), QDirIterator::NoIteratorFlags);
    while (it.hasNext())
    {
        QFileInfo currFileInfo(it.next());
        if (currFileInfo.isDir()) continue;

        //qDebug() << "Текущий файл: " << currFileInfo.fileName();
        if (i == 1)
        {
            //qDebug() << fileInfo.fileName() + " == " + QFileInfo(currFileInfo.fileName()).completeBaseName();
            if (fileInfo.fileName() == QFileInfo(currFileInfo.fileName()).completeBaseName())
            {
                isUnique = false;
                newFilePath = getUniqueFilePath(filePath, i + 1);
            }
        }
        else
        {
            //qDebug() << fileInfo.fileName() + "_" + QString::number(i) + " == " + QFileInfo(currFileInfo.fileName()).completeBaseName();
            if (fileInfo.fileName() + "_" + QString::number(i) == QFileInfo(currFileInfo.fileName()).completeBaseName())
            {
                isUnique = false;
                newFilePath = getUniqueFilePath(filePath, i + 1);
            }
        }
    }

    if (isUnique && i != 1)
    {
        return (newFilePath + "_" + QString::number(i));
    }
    else
    {
        return newFilePath;
    }
}

QString translit(const QString str)
{
    QString out = "";
    QMap<QChar, QString> map;
    map[L'а'] = "a";
    map[L'б'] = "b";
    map[L'в'] = "v";
    map[L'г'] = "g";
    map[L'д'] = "d";
    map[L'е'] = "e";
    map[L'ё'] = "io";
    map[L'ж'] = "zh";
    map[L'з'] = "z";
    map[L'и'] = "i";
    map[L'й'] = "y";
    map[L'к'] = "k";
    map[L'л'] = "l";
    map[L'м'] = "m";
    map[L'н'] = "n";
    map[L'о'] = "o";
    map[L'п'] = "p";
    map[L'р'] = "r";
    map[L'с'] = "s";
    map[L'т'] = "t";
    map[L'у'] = "u";
    map[L'ф'] = "f";
    map[L'х'] = "h";
    map[L'ц'] = "c";
    map[L'ч'] = "ch";
    map[L'ш'] = "sh";
    map[L'щ'] = "sch";
    map[L'ъ'] = "";
    map[L'ы'] = "y";
    map[L'ь'] = "";
    map[L'э'] = "e";
    map[L'ю'] = "yu";
    map[L'я'] = "ya";

    for (int i = 0; i < str.size(); i++)
    {
        QString newChar = "";
        QString mapValue = map.value(str[i]);
        if (mapValue != "")
        {
            newChar = mapValue;
        }
        else
        {
            newChar = str[i];
        }

        out += newChar;
    }

    return out;
}

IO_Shape::IO_Shape(){}

void IO_Shape::WriteShape()
{
    Geometry::SetShapeFile("test_poligons");
}

void IO_Shape::WriteShape(QString featureType, xml_header header, QVector<Feature> features, QString filePath, bool isInvertXY, bool isAutoDirtyFix)
{
    Geometry::SetSmartReverse(isAutoDirtyFix); // TO-DO: Привязать к чек-боксу
    QString uniqueFilePath = getUniqueFilePath(filePath);
    char *filePathC = (char*)malloc(uniqueFilePath.length() + 1);
    strcpy(filePathC, uniqueFilePath.toStdString().c_str()); // TO-DO: Можно в будущем изменить на более безопасную функцию
    Geometry::SetShapeFile(filePathC);
    short int type = 0;

    if (featureType == "MultiPolygon")
    {
        type = SHPT_POLYGON;
    }
    else
    {
        qDebug(logDebug()) << "Некорректный тип объекта!";
        return;
    }

    for (int i = 0; i < features.count(); i++)
    {
        Feature currFeature = features.at(i);
        Geometry polygon(type);
        QVector<Shell> shells = currFeature.getShells();
        for (int j = 0; j < shells.count(); j++)
        {
            Shell currShell = shells.at(j);
            GeometryObject coordinates = currShell.getCoordinates();
            polygon.StartSubpart();
            for (int k = 0; k < coordinates.count(); k++)
            {
                Coordinate currCoordinate = coordinates.at(k);

                if (isInvertXY)
                {
                    polygon.PointPush(currCoordinate.getY(), currCoordinate.getX());
                }
                else
                {
                    polygon.PointPush(currCoordinate.getX(), currCoordinate.getY());
                }
            }
            QVector<GeometryObject> holes = currShell.getHoles();
            for (int k = 0; k < holes.count(); k++)
            {
                GeometryObject currHole = holes.at(k);
                polygon.StartHole();

                for (int l = 0; l < currHole.count(); l++)
                {
                    Coordinate currHoleCoordinate = currHole.at(l);
                    if (isInvertXY)
                    {
                        polygon.PointPush(currHoleCoordinate.getY(), currHoleCoordinate.getX());
                    }
                    else
                    {
                        polygon.PointPush(currHoleCoordinate.getX(), currHoleCoordinate.getY());
                    }
                }

                polygon.EndHole();
            }
        }

        QVector<Attribute> attributes = currFeature.getAttributes();
        for (int j = 0; j < attributes.count(); j++)
        {
            Attribute currAttribute = attributes.at(j);
            QString type;

            for (int k = 0; k < header.count(); k++)
            {
                QPair<QString, QString> currPair = header.at(k);
                if (currPair.first == currAttribute.getName())
                {
                    type = currPair.second;
                }
            }

            if (type == "String")
            {
                polygon.AddAttribute(FTString, currAttribute.getName(), currAttribute.getValue());
            }
            else if (type == "Number") // CHECK: Тут double или int?
            {
                polygon.AddAttribute(FTDouble, currAttribute.getName(), currAttribute.getValue());
            }
        }

        polygon.WriteToShapeFile();
    }

    Geometry::SaveShapeFile();
}

const char* IO_Shape::typeStr(int def){ // TO-DO: Только заготовка под определение GeometryType из XML, надо будет менять
    switch (def){
    case SHPT_NULL:
        return "SHPT_NULL";
    case SHPT_POINT:
        return "SHPT_POINT";
    case SHPT_ARC:
        return "SHPT_ARC";
    case SHPT_POLYGON:
        return "SHPT_POLYGON";
    case SHPT_MULTIPOINT:
        return "SHPT_MULTIPOINT";
    case SHPT_POINTZ:
        return "SHPT_POINTZ";
    case SHPT_ARCZ:
        return "SHPT_ARCZ";
    case SHPT_POLYGONZ:
        return "SHPT_POLYGONZ";
    case SHPT_MULTIPOINTZ:
        return "SHPT_MULTIPOINTZ";
    case SHPT_POINTM:
        return "SHPT_POINTM";
    case SHPT_ARCM:
        return "SHPT_ARCM";
    case SHPT_POLYGONM:
        return "SHPT_POLYGONM";
    case SHPT_MULTIPOINTM:
        return "SHPT_MULTIPOINTM";
    case SHPT_MULTIPATCH:
        return "SHPT_MULTIPATCH";
    default:
        return "NOT RECOGNIZED";
    }
}
