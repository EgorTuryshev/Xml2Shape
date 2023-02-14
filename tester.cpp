#include "tester.h"

void tester::testReadingHeaders(QString filePath)
{
    xml_parser::setPath(filePath);
    xml_header header = xml_parser::readTypeHeader();

    qDebug() << "Start testReadingHeaders()";
    qDebug() << "Count = " << header.count();
    for (int i = 0; i < header.count(); i++) {
        qDebug() << header[i].first << " " << header[i].second;
    }
    qDebug() << "End" << "\n";
}

void tester::testReadingFeatureType(QString filePath)
{
    xml_parser::setPath(filePath);

    qDebug() << "Start testReadingFeatureType()";
    qDebug() << xml_parser::readFeatureType();
    qDebug() << "End" << "\n";
}

void tester::testReadingAttributes(QString filePath)
{
    xml_parser::setPath(filePath);

    qDebug() << "Start testReadingAttributes()";
    features_attributes attrs = xml_parser::readAttributes();

    for (int i = 0; i < attrs.count(); i++) {
        qDebug() << "Feature[" << i << "]: " << attrs.at(i);
    }

    qDebug() << "End" << "\n";
}

void tester::testReadingDirectories()
{
    QString lastdir = ".";
    QString itdir = ".";
    QVector <QFile> files;

    QDirIterator it(QDir::currentPath() + "/templates", QDir::Files, QDirIterator::Subdirectories);

    if (it.hasNext()) {
        it.next();
        itdir = QFileInfo(it.filePath()).dir().dirName();
        lastdir = itdir;
        qDebug(logDebug()) << "ИМЯ ПАПКИ" << itdir;
        qDebug(logDebug()) << "ИМЯ ФАЙЛА" << it.fileName();
    }

    while (it.hasNext()) {
        it.next();
        itdir = QFileInfo(it.filePath()).dir().dirName();

        if (itdir != lastdir) {
            qDebug(logDebug()) << "dirChanged";
            lastdir = itdir;
        }

        qDebug(logDebug()) << "ИМЯ ПАПКИ" << itdir;
        qDebug(logDebug()) << "ИМЯ ФАЙЛА" << it.fileName();
    }
}

void tester::testReadingFeatures(QString filePath)
{
    xml_parser::setPath(filePath);
    QVector<Feature> features = xml_parser::readFeautures();

    for (int i = 0; i < features.count(); i++)
    {
        Feature currFeature = features.at(i);
        QVector<Attribute> currAttrs = currFeature.getAttributes();
        Shells currShells = currFeature.getShells();

        for (int j = 0; j < currAttrs.count(); j++)
        {
            Attribute currAttr = currAttrs.at(j);
            qDebug() << currAttr.getName() + " " + currAttr.getValue();
        }

        for (int j = 0; j < currShells.count(); j++)
        {
            QVector<Coordinate> currShell = currShells.at(j);

            for (int k = 0; k < currShell.count(); k++)
            {
                Coordinate currCoord = currShell.at(k);
                qDebug() << QString::number(currCoord.getX()) + " " + QString::number(currCoord.getY())
                            + " " + QString::number(currCoord.getOrdNumber());
            }
        }
    }
}

tester::tester()
{

}
