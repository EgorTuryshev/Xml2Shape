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
    QString program = "../Xml2Shape/xslt_processor_app/xslt_processor.exe"; // TO-DO: перед релизом изменить путь
    params << xmlInfo.absoluteFilePath() << xsltInfo.absoluteFilePath();
    p.start(program, params);
    p.waitForFinished();
    QString output(p.readAllStandardOutput());
    return output;
}

QString xslt_processor::processXSLT_data(QString xmlFileName, QString xmlData, QString xsltPath)
{
    QSharedMemory mem;
    mem.setKey("xmlData");
    QByteArray xmlDataBytes = xmlData.toUtf8();
    if (!mem.create(xmlDataBytes.size())) // Занимаем память
    {
        qDebug(logDebug()) << "Не удалось выделить общую память для процесса!";
        return "";
    }
    char *data = (char*)mem.data(); // Указатель на общую память
    memcpy(data, xmlDataBytes.data(), strlen(xmlDataBytes.data() + 1)); // +1?
    mem.unlock(); // Заканчиваем работу с общей памятью

    // Более медленный способ
    /*if (mem.lock())
    {
        for (int i = 0; i < xmlDataBytes.size(); i++)
        {
            data[i] = xmlDataBytes[i];
        }
        mem.unlock();
    }*/

    QProcess p;
    QStringList params;
    QFileInfo xsltInfo (xslt_processor::cwd + xsltPath); // ?
    QString program = "../xslt_processor_data_app/xslt_processor_data.exe"; // TO-DO: перед релизом изменить путь
    //QString program = "./xslt_processor_data_app/xslt_processor_data.exe";
    params << xmlFileName << xsltInfo.absoluteFilePath();
    p.start(program, params);
    p.waitForFinished();
    QString output(p.readAllStandardOutput());
    return output;
}

void xslt_processor::setcwd(QString path)
{
    xslt_processor::cwd = path;
}
