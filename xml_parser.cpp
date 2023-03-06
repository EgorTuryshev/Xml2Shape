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

void xml_parser::setXML(const QString xml_str)
{
    QDomDocument xmlDocument;
    xmlDocument.setContent(xml_str);
    xml_parser::root = xmlDocument.documentElement();
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

QVector<Feature> xml_parser::readFeautures()
{
    QVector<Feature> features;

    QDomNodeList featuresEls = xml_parser::root.firstChildElement("Features").elementsByTagName("Feature");
    for (int i = 0; i < featuresEls.count(); i++) {
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

        if (currFeatureGeometryEl.isNull())
        {
            continue;
        }

        QDomNodeList shellsDom = currFeatureGeometryEl.elementsByTagName("Shell");

        if (shellsDom.count() == 0)
        {
            continue;
        }

        QVector<Shell> currShellsVect;

        for (int j = 0; j < shellsDom.count(); j++)
        {
            QDomElement currShellEl = shellsDom.at(j).toElement();

            QDomNodeList coordsDom = currShellEl.elementsByTagName("Coordinate");
            GeometryObject coordinates;
            for (int k = 0; k < coordsDom.count(); k++)
            {
                QDomElement currCoordEl = coordsDom.at(k).toElement();

                if (currCoordEl.parentNode().nodeName() == "Hole") // ?
                {
                    break;
                }

                double x = currCoordEl.attribute("x").toDouble();
                double y = currCoordEl.attribute("y").toDouble();
                unsigned int ord_number = currCoordEl.attribute("ord_number").toUInt();
                Coordinate currCoordinate(x, y, ord_number);
                coordinates.push_back(currCoordinate);
            }

            QDomElement holesEl = currShellEl.firstChildElement("Holes");
            QDomNodeList holesDom = holesEl.elementsByTagName("Hole");
            QVector<GeometryObject> holes;
            for (int k = 0; k < holesDom.count(); k++)
            {
                QDomElement currHoleEl = holesDom.at(k).toElement();
                QDomNodeList currHoleCoordsDom = currHoleEl.elementsByTagName("Coordinate");
                GeometryObject currHole;
                for (int l = 0; l < currHoleCoordsDom.count(); l++)
                {
                    QDomElement currHoleCoordEl = currHoleCoordsDom.at(l).toElement();
                    double x = currHoleCoordEl.attribute("x").toDouble();
                    double y = currHoleCoordEl.attribute("y").toDouble();
                    unsigned int ord_number = currHoleCoordEl.attribute("ord_number").toUInt();
                    Coordinate currCoordinate(x, y, ord_number);
                    currHole.push_back(currCoordinate);
                }
                holes.push_back(currHole);
            }

            Shell currShell(coordinates, holes);
            currShellsVect.push_back(currShell);
        }

        Feature feauture(currAttrsVect, currShellsVect);
        features.push_back(feauture);
    }

    return features;
}
