#include "appcore.h"

Appcore::Appcore(QObject *parent) : QObject(parent)
{

}
void Appcore::test(){
    xslt_processor::setcwd("./");
    QString processedXML_str = xslt_processor::processXSLT("../Xml2Shape/samples/test.xml", "../Xml2Shape/samples/kpt.xsl");
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

void Appcore::autoClearingLogChanged(CheckState newState)
{

}
