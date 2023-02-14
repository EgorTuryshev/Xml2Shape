#include "xslt_processor.h"
#include <QFile>
#include <QtXmlDepends>
#include "loggingcategories.h"

xslt_processor::xslt_processor()
{
    this->cwd = "";
}

QString xslt_processor::processXSLT(QString xmlPath, QString xsltPath) {
    QProcess p;
    QStringList params;
    QFileInfo info ("../Xml2Shape/xslt_processor.py"); // Возможно, надо будет изменить в будущем
    QFileInfo xmlInfo (this->cwd + xmlPath); // ?
    QFileInfo xsltInfo (this->cwd + xsltPath); // ?
    params << info.absoluteFilePath() << xmlInfo.absoluteFilePath() << xsltInfo.absoluteFilePath();
    p.start("python", params);
    p.waitForFinished();
    QString output(p.readAllStandardOutput());
    return output;
}

void xslt_processor::setcwd(QString path) {
    this->cwd = path;
}
