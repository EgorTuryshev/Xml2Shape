#include "xslt_processor.h"
#include <QFile>
#include <QtXmlDepends>
#include "loggingcategories.h"

QString xslt_processor::cwd = "";

QString xslt_processor::processXSLT(QString xmlPath, QString xsltPath) {
    QProcess p;
    QStringList params;
    QFileInfo info ("../Xml2Shape/xslt_processor.py"); // Возможно, надо будет изменить в будущем
    QFileInfo xmlInfo (xslt_processor::cwd + xmlPath); // ?
    QFileInfo xsltInfo (xslt_processor::cwd + xsltPath); // ?
    params << info.absoluteFilePath() << xmlInfo.absoluteFilePath() << xsltInfo.absoluteFilePath();
    p.start("python", params);
    p.waitForFinished();
    QString output(p.readAllStandardOutput());
    return output;
}

void xslt_processor::setcwd(QString path) {
    xslt_processor::cwd = path;
}
