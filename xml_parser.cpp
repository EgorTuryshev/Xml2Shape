#include "xml_parser.h"

QString xml_parser::path = "";
QDomElement xml_parser::root = QDomElement();

QDomElement xml_parser::getFirstDomElByName(QString elementName, QDomElement rootElement)
{
    QDomElement element = rootElement.firstChildElement(elementName);
    if (!element.isNull()) {
        return element;
    }
    else {
        QDomElement child = rootElement.firstChild().toElement();

        while (!child.isNull()) {
            element = xml_parser::getFirstDomElByName(elementName, child);

            if (!element.isNull()) {
                return element;
            }

            child = rootElement.nextSibling().toElement();
        }
    }

    return element;
}



QString xml_parser::getPath() {
    return xml_parser::path;
}

void xml_parser::setPath(QString path) {
    xml_parser::path = path;

    QDomDocument xmlDocument;
    QFile xmlFile(xml_parser::path);
    if (!xmlFile.open(QIODevice::ReadOnly ))
    {
        QString errStr = "XML-файл не может быть открыт!";
        qWarning(logWarning()) << errStr;
    }

    xmlDocument.setContent(&xmlFile);
    xml_parser::root = xmlDocument.documentElement();
    xmlFile.close();
}

QString xml_parser::readFeatureType()
{
    QDomElement featureTypeEl = xml_parser::getFirstDomElByName("FeatureType", xml_parser::root);
    return featureTypeEl.attribute("geometry_type", "");
}

xml_header xml_parser::readTypeHeader()
{
    xml_header header;
    QDomElement attrRoot = xml_parser::getFirstDomElByName("Attributes", xml_parser::root);
    QDomElement child = attrRoot.firstChildElement("Attribute");

    while (child.isNull() == false)
    {
        QString name = child.attribute("name", "name");
        QString type = child.attribute("type", "String");
        QPair<QString, QString> pair(name, type);
        header.push_back(pair);
        child = child.nextSiblingElement("Attribute");
    }

    return header;
}

features_attributes xml_parser::readAttributes()
{
    features_attributes attributes;

    QDomElement featuresEl = xml_parser::root.firstChildElement("Features");
    QDomNodeList features = featuresEl.elementsByTagName("Feature");

    for (int i = 0; i < features.count(); i++) { // count()?
        QDomElement currFeature = features.at(i).toElement();
        QDomElement currAttrsEl = currFeature.firstChildElement("Attributes");
        QDomNodeList currAttrs = currAttrsEl.elementsByTagName("Attribute");
        QVector<QString> currAttrsVect;

        for (int j = 0; j < currAttrs.count(); j++) {
            QDomElement currAttr = currAttrs.at(j).toElement();
            currAttrsVect.push_back(currAttr.text());
        }
        attributes.push_back(currAttrsVect);
    }

    return attributes;
}

Shells xml_parser::readCoordinates()
{
    Shells coordinates;

    return coordinates;
}

QVector<Feature> xml_parser::readFeautures()
{
    QVector<Feature> features;

    QDomNodeList featuresEls = xml_parser::root.firstChildElement("Features").elementsByTagName("Feature");
    for (int i = 0; i < featuresEls.count(); i++) { // count()?
        QDomElement currFeature = featuresEls.at(i).toElement();
        QDomElement currAttrsEl = currFeature.firstChildElement("Attributes");
        QDomNodeList currAttrs = currAttrsEl.elementsByTagName("Attribute");
        QVector<Attribute> currAttrsVect;

        for (int j = 0; j < currAttrs.count(); j++) {
            QDomElement currAttrEl = currAttrs.at(j).toElement();
            Attribute currAttr(currAttrEl.attribute("name"), currAttrEl.text());
            currAttrsVect.push_back(currAttr);
        }

        QDomElement currFeatureGeometryEl = currFeature.firstChildElement("Geometry");
        QDomNodeList shellsDom = currFeatureGeometryEl.elementsByTagName("Shell");
        Shells currShellsVect;

        for (int j = 0; j < shellsDom.count(); j++)
        {
            QDomElement currShellEl = shellsDom.at(j).toElement();
            QDomNodeList currShellCoordsDom = currShellEl.elementsByTagName("Coordinate");
            QVector<Coordinate> currShellCoordsVect;

            for (int k = 0; k < currShellCoordsDom.count(); k++)
            {
                QDomElement currShellCoordEl = currShellCoordsDom.at(k).toElement();
                double x = currShellCoordEl.attribute("x").toDouble();
                double y = currShellCoordEl.attribute("y").toDouble();
                unsigned int ord_number = currShellCoordEl.attribute("ord_number").toUInt();
                Coordinate currCoord(x, y, ord_number);
                currShellCoordsVect.push_back(currCoord);
            }
            currShellsVect.push_back(currShellCoordsVect);
        }

        Feature feauture(currAttrsVect, currShellsVect);
        features.push_back(feauture);
    }

    return features;
}
