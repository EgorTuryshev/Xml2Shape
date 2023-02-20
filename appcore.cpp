#include "appcore.h"

QString removeExt(QString str)
{
    int dotInd = 0;
    for (int i = str.length() - 1; i >= 0; i--)
    {
        if (str[i] == '.')
        {
            dotInd = i;
            break;
        }
    }

    return (str.first(dotInd));
}

Appcore::Appcore(QObject *parent) : QObject(parent) { }

void Appcore::test(QString xmlFilePath, QString xslFilePath, QString targetPath){
    if (xmlFilePath.first(8) == "file:///")
    {
        xmlFilePath = xmlFilePath.remove(0, 8);
    }
    if (xslFilePath.first(8) == "file:///")
    {
        xslFilePath = xslFilePath.remove(0, 8);
    }
    if (targetPath.first(8) == "file:///")
    {
        targetPath = targetPath.remove(0, 8);
    }

    qDebug(logDebug()) << xmlFilePath << " " << xslFilePath;

    QString filePath = targetPath + "/" + QFileInfo(xmlFilePath).completeBaseName(); // TO-DO: Доработать, чтобы имя было уникальным для директории
    qDebug(logDebug()) << filePath;
    xslt_processor::setcwd("");
    QString processedXML_str = xslt_processor::processXSLT(xmlFilePath, xslFilePath);
    xml_parser::setXML(processedXML_str);
    QString featureType = xml_parser::readFeatureType();
    xml_header header = xml_parser::readTypeHeader();
    QVector<Feature> features = xml_parser::readFeautures();

    IO_Shape s;
    //s.WriteShape();
    s.WriteShape(featureType, header, features, filePath);

}
void Appcore::openLog()
{
    QStringList p;
    p << "logFile.txt";
    QProcess::execute("notepad.exe", p);
}
void Appcore::clearLog(bool isAutoClearing)
{
    if (isAutoClearing) qWarning(clr()) << "Лог был автоматически очищен программой";
    else qWarning(clr()) << "Лог был чищен пользователем";
}
