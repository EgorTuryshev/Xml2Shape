#include "appcore.h"

Appcore::Appcore(QObject *parent) : QObject(parent)
{

}
void Appcore::test(QString xmlFilePath, QString xslFilePath){
    if (xmlFilePath.first(8) == "file:///")
    {
        xmlFilePath = xmlFilePath.remove(0, 8);
    }
    if (xslFilePath.first(8) == "file:///")
    {
        xslFilePath = xslFilePath.remove(0, 8);
    }

    qDebug(logDebug()) << xmlFilePath << " " << xslFilePath;
    xslt_processor::setcwd("");
    QString processedXML_str = xslt_processor::processXSLT(xmlFilePath, xslFilePath);
    xml_parser::setXML(processedXML_str);
    QString featureType = xml_parser::readFeatureType();
    xml_header header = xml_parser::readTypeHeader();
    QVector<Feature> features = xml_parser::readFeautures();

    IO_Shape s;
    //s.WriteShape();
    s.WriteShape(featureType, header, features);

}
void Appcore::openLog()
{
    QStringList p;
    p << "logFile.txt";
    QProcess::execute("notepad.exe", p);
}
void Appcore::clearLog()
{
    qWarning(clr()) << "Лог был очищен пользователем";
}
