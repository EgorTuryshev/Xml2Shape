#include "io_shape.h"
#include <stdlib.h>
#include <iostream>
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QDebug>
#include <QtXml>
#include "LoggingCategories.h"
#include "fs_category.h"
#include "shapelib/shapefil.h"
#include <geometry.h>
#include "xslt_processor.h"
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

    QString  lastdir = ".";
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
        categories.last().Debug_DisplayCategory();
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
