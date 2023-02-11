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
#include "shapelib/shapefil.h"
#include <geometry.h>
#include "xslt_processor.h"

using namespace std;

IO_Shape::IO_Shape()
{
}
void IO_Shape::WriteShape() // Тестовая функция записи shp + shx
{
    xslt_processor xslt;
    xslt.setcwd("../Xml2Shape/samples/");
    xslt.processXSLT("test.xml", "kpt.xsl");

    qDebug(logDebug()) << "lolkek";
    double n1[] = {50};
    double n2[] = {60};
    double n3[] = {0};
    SHPHandle shp = SHPCreate("123", SHPT_POINT);
    SHPWriteObject(shp, -1, SHPCreateSimpleObject(SHPT_POINT, 1, n1, n2, n3));
    SHPClose(shp);
    //SHPCreateObject()
}

void IO_Shape::ReadXml()
{
    QDomDocument TargetXML;

    QFile xmlFile("Sample.xml");
    if (!xmlFile.open(QIODevice::ReadOnly ))
    {
        qWarning(logWarning()) << "Xml не может быть считан";
    }

    TargetXML.setContent(&xmlFile);
    xmlFile.close();

    QDomElement root = TargetXML.documentElement();
    QDomElement node = root.firstChild().toElement();

    QString uiData = "";

    //Сначала нужно будет определить тип элемента Geometry

    while(node.isNull() == false)
        {
            qDebug(logDebug()) << node.tagName();
            if(node.tagName() == "feature"){ //+ нужна проверка на дырки
                while(!node.isNull()){

                    QString id = node.attribute("id","0");  //Атрибуты в XML называются иначе, надо будет менять
                    QString name = node.attribute("name","name");
                    QString number = node.attribute("number","number");

                    uiData.append(id).append(" - ").append(name).append(" - ").append(number).append("\n");

                    node = node.nextSibling().toElement();
                }
            }
            node = node.nextSibling().toElement();
        }
    //Это очень простой пример парсинга, надо углябляться

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
