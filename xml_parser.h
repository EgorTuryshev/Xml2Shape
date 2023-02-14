#ifndef XML_PARSER_H
#define XML_PARSER_H

#include <QtXml>
#include <QFile>
#include "loggingcategories.h"
#include "feature.h"

using xml_header = QVector< QPair<QString, QString> >;
using features_attributes = QVector< QVector<QString> >;

class xml_parser
{
private:
    static QString path; // Путь к XML-файлу
    static QDomElement root; // Корневой элемент XML-файла

    xml_parser() { }
    /// Возвращает первый встреченный в родителе элемент по заданному имени (рекурсивный поиск)
    static QDomElement getFirstDomElByName(QString elementName, QDomElement rootElement = xml_parser::root);
public:
    static QString getPath();
    static void setPath(const QString path);
    static QString readFeatureType();
    /// Читает описание SHP-атрибутов
    static xml_header readTypeHeader();
    /// Считывает теги Attribute (устаревший метод)
    static features_attributes readAttributes();
    /// Считывает теги Coordinate (устаревший метод)
    static Shells readCoordinates();
    /// Считывает теги Feature
    static QVector<Feature> readFeautures();
};

#endif // XML_PARSER_H
