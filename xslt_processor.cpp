#include "xslt_processor.h"
#include <QFile>
#include <QtXmlDepends>

QString xslt_processor::cwd = "";

QString xslt_processor::processXSLT(QString xmlPath, QString xsltPath)
{
    QProcess p;
    QStringList params;
    QFileInfo xmlInfo (xslt_processor::cwd + xmlPath); // ?
    QFileInfo xsltInfo (xslt_processor::cwd + xsltPath); // ?
    //QString program = "../Xml2Shape/xslt_processor_app/xslt_processor.exe"; // TO-DO: перед релизом изменить путь
    QString program = "../xslt_processor_new_app/xslt_processor.exe";
    params << xmlInfo.absoluteFilePath() << xsltInfo.absoluteFilePath();
    p.start(program, params);
    p.waitForFinished();
    QString output(p.readAllStandardOutput());
    return output;
}

void xslt_processor::setcwd(QString path)
{
    xslt_processor::cwd = path;
}
