#include "appcore.h"

Appcore::Appcore(QObject *parent) : QObject(parent)
{

}
void Appcore::test(){
    xslt_processor::setcwd("./");
    QString processedXML_str = xslt_processor::processXSLT("../Xml2Shape/samples/big.xml", "../Xml2Shape/samples/kpt.xsl");
    QFile processedXML_file("processed.xml");
    processedXML_file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream writeStream(&processedXML_file);
    writeStream << processedXML_str;
    processedXML_file.close();
    qDebug(logDebug()) << "After writing processed xml";

    xml_parser::setXML(processedXML_str);
    QString featureType = xml_parser::readFeatureType();
    xml_header header = xml_parser::readTypeHeader();
    QVector<Feature> features = xml_parser::readFeautures();

    IO_Shape s;
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
