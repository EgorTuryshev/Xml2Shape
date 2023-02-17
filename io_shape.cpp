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
#include <fs_property_manager.h>

using namespace std;

IO_Shape::IO_Shape()
{
}
void IO_Shape::WriteShape() // Тестовая функция записи shp + shx
{
    //теперь НИКАК не взаимодействуем с библиотекой напрямую (кроме типов атрибутов)
    //теперь просто называем SHAPEFILE и не создаем его с типом
    Geometry::SetShapeFile("nice_implementation");
    //в конструкторе теперь необходим тип объекта
    Geometry polygon(SHPT_POLYGON);

    /*polygon.PointPush(72, 57);
    polygon.PointPush(89, 57);
    polygon.PointPush(92, 68);
    polygon.PointPush(75, 67);
    polygon.PointPush(72, 57);*/

    polygon.PointPush(75, 67);
    polygon.PointPush(92, 68);
    polygon.PointPush(89, 57);
    polygon.PointPush(72, 57);
    polygon.PointPush(75, 67);

    //просто изменение синтаксиса
    /*polygon.StartHole();
    polygon.PointPush(81, 63);
    polygon.PointPush(84, 63);
    polygon.PointPush(84, 61);
    polygon.PointPush(81, 62);
    polygon.PointPush(81, 63);
    polygon.EndHole();*/

    //теперь не создаем поля таблицы, а сразу присваиваем атрибуты
    polygon.AddAttribute(FTString, "header", "some_val");
    polygon.AddAttribute(FTString, "second", "another_val");
    polygon.AddAttribute(FTDouble, "double", "4567825.523");
    polygon.AddAttribute(FTInteger, "int", "696969");

    //записываем фигуру сразу в весь формат SHP + DBF + SHX (в оперативную память)
    polygon.WriteToShapeFile();

    //
    /*Geometry new_polygon(SHPT_POLYGON);

    new_polygon.PointPush(65, 75);
    new_polygon.PointPush(97, 72);
    new_polygon.PointPush(100, 47);
    new_polygon.PointPush(63, 52);
    new_polygon.PointPush(65, 75);

    new_polygon.AddAttribute(FTString, "header", "i_am_new");

    new_polygon.WriteToShapeFile();*/

    //перенос всего записанного выше в файл
    Geometry::SaveShapeFile();

    /*xslt_processor xslt;
    xslt.setcwd("../Xml2Shape/samples/");
    xslt.processXSLT("test.xml", "kpt.xsl");

    double n1[] = {50};
    double n2[] = {60};
    double n3[] = {0};
    SHPHandle shp = SHPCreate("123", SHPT_POINT);
    SHPWriteObject(shp, -1, SHPCreateSimpleObject(SHPT_POINT, 1, n1, n2, n3));
    SHPClose(shp);*/

    //qDebug(logDebug()) << fs_property_manager::GetPropertyValue("./templates/dir/Текстовый документ.txt", "Description");
    //qDebug(logDebug()) << fs_property_manager::GetFileName("./templates/dir/Текстовый документ.txt");
    //return;

    /*QString  lastdir = ".";
    QString itdir = ".";
    QVector <QString> files;
    QVector <fs_category> categories;
    QVector <fs_xslt> category_xslts;
    QDirIterator it(QDir::currentPath() + "/templates", QDir::Files, QDirIterator::Subdirectories);
    qDebug(logDebug()) << it.hasNext();

    if (it.hasNext())
    {
            it.next();
            itdir = QFileInfo(it.filePath()).dir().dirName();
            lastdir = itdir;
    }

        while (it.hasNext())
        {
            if (itdir != lastdir)
            {
                qDebug(logDebug()) << "ВХОД В ПАРСЕР";
                for(int i = 0; i < files.count(); i++){
                    if(fs_property_manager::GetFileName(files[i]) != "root.desc")
                    {
                        qDebug(logDebug()) << "ВХОД В ФАЙЛЫ";
                        QString name = fs_property_manager::GetPropertyValue(files[i], "Name");
                        qDebug(logDebug()) << "ЕСТЬ ИМЯ" << name;
                        QString description = fs_property_manager::GetPropertyValue(files[i], "Description");
                        qDebug(logDebug()) << "ЕСТЬ ОПИСАНИЕ";
                        category_xslts.push_back(fs_xslt(name, fs_property_manager::GetFileName(files[i]), description));
                        qDebug(logDebug()) << "ЕСТЬ КАТЕГОРИЯ";
                    }
                    else
                    {
                        qDebug(logDebug()) << "ВХОД В КАТЕГОРИИ";
                        QString name = fs_property_manager::GetPropertyValue(files[i], "Name");
                        categories.push_back(fs_category(name, files[i]));
                        //Действия при категории
                    }

                }
                categories.last().SetXslts(category_xslts);
                categories.last().Debug_DisplayCategory();
                qDebug(logDebug()) << "dirChanged";
                lastdir = itdir;
                files.clear();
            }
            else
            {
                qDebug(logDebug()) << "БЫЛ ПУШ" << itdir;
                files.push_back(it.filePath());
                qDebug(logDebug()) << files.last();
            }

            qDebug(logDebug()) << "ИМЯ ПАПКИ" << itdir;
            qDebug(logDebug()) << "ИМЯ ФАЙЛА" << it.fileName();
            it.next();
            itdir = QFileInfo(it.filePath()).dir().dirName();
        }
        categories.last().Debug_DisplayCategory();*/
}

void IO_Shape::WriteShape(QString featureType, xml_header header, QVector<Feature> features)
{
    Geometry::SetShapeFile("test_shape");
    short int type = 0;

    if (featureType == "MultiPolygon") // Добавить другие типы
    {
        type = SHPT_POLYGON;
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
                polygon.PointPush(currCoordinate.getX(), currCoordinate.getY());
            }
            QVector<GeometryObject> holes = currShell.getHoles();
            for (int k = 0; k < holes.count(); k++)
            {
                GeometryObject currHole = holes.at(k);
                polygon.StartHole();

                for (int l = 0; l < currHole.count(); l++)
                {
                    Coordinate currHoleCoordinate = currHole.at(l);
                    polygon.PointPush(currHoleCoordinate.getX(), currHoleCoordinate.getY());
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
            else if (type == "Number") // Тут double или int?
            {
                polygon.AddAttribute(FTDouble, currAttribute.getName(), currAttribute.getValue());
            }
        }

        polygon.WriteToShapeFile();
    }

    Geometry::SaveShapeFile();
}

const char* IO_Shape::typeStr(int def){ //Только заготовка под определение GeometryType из XML, надо будет менять
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
