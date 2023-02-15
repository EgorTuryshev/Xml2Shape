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

void tester::testReadingFeatures(QString filePath) // Допилить
{
    xml_parser::setPath(filePath);
    QVector<Feature> features = xml_parser::readFeautures();
}

tester::tester()
{

}
