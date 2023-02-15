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
    /// Задает XML-документ, с которым класс будет работать (необходимо передать путь к файлу)
    static void setPath(const QString path);
    /// Задает XML-документ, с которым класс будет работать (необходимо передать XML-разметку в виде строки)
    static void setXML(const QString xml_str);
    /// Читает тип геометрического объекта
    static QString readFeatureType();
    /// Читает описание SHP-атрибутов
    static xml_header readTypeHeader();
    /// Считывает теги Feature
    static QVector<Feature> readFeautures();
};

#endif // XML_PARSER_H
