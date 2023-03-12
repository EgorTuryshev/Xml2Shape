#include "xslt_processor.h"
#include <QFile>
#include <QtXmlDepends>

QString xslt_processor::cwd = "";

QString xslt_processor::processXSLT(QString xmlPath, QString xsltPath)
{
    QProcess p;

    QSharedMemory xml_mem;
    xml_mem.setKey("xmlPath");
    if (!xml_mem.create(xmlPath.toUtf8().size()))
    {
        return "";
    }
    char *xml_memDataPointer = (char*)xml_mem.data();
    memcpy(xml_memDataPointer, xmlPath.toUtf8().data(), strlen(xmlPath.toUtf8().data()));
    xml_mem.unlock();

    QSharedMemory xslt_mem;
    xslt_mem.setKey("xsltPath");
    if (!xslt_mem.create(xmlPath.toUtf8().size()))
    {
        return "";
    }
    char *xslt_memDataPointer = (char*)xslt_mem.data();
    memcpy(xslt_memDataPointer, xsltPath.toUtf8().data(), strlen(xsltPath.toUtf8().data()));
    xslt_mem.unlock();

    QString program = "../Xml2Shape/xslt_processor_zip_app/xslt_processor_zip.exe"; // TO-DO: перед релизом изменить путь
    p.start(program);
    p.waitForFinished();
    QString output(p.readAllStandardOutput());
    return output;
}

void xslt_processor::setcwd(QString path)
{
    xslt_processor::cwd = path;
}
