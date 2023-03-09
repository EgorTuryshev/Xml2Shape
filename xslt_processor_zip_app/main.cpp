#include <QCoreApplication>
#include <QtGui/private/qzipreader_p.h>
#include <QXmlQuery>
#include <QString>
#include <QFileInfo>
#include <QSharedMemory>
#include <iostream>

QString getFileNameWithoutPath(QString path)
{
    return path.section("/", -1, -1); // ?
}

QByteArray getDataBytesFromSharedMem(QString keyName)
{
    QSharedMemory mem;
    QByteArray dataBytes;
    mem.setKey(keyName); // CHECK: Могут быть проблемы на MacOS, там названия должны быть стандартизированы
    if (!mem.attach()) // Подключаемся к уже выделенной общей памяти
    {
        return QByteArray();
    }
    char *data = (char*)mem.data(); // Указатель на общую память
    if (mem.lock())
    {
        for (int i = 0; data[i]; i++)
            dataBytes.append(data[i]);
        mem.unlock();
    }
    mem.detach();

    return dataBytes;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString xmlPath(getDataBytesFromSharedMem("xmlPath"));
    QString xsltPath(getDataBytesFromSharedMem("xsltPath"));

    if (xmlPath.isEmpty())
    {
        return 1;
    }

    if (xsltPath.isEmpty())
    {
        return 1;
    }

    QString out = "";
    QFile xslFile(xsltPath);
    QFileInfo xmlFileInfo(xmlPath);
    QXmlQuery query(QXmlQuery::XSLT20);

    if (!xslFile.open(QIODevice::ReadOnly))
    {
        return 1;
    }

    if (xmlFileInfo.suffix() == "zip")
    {
        QZipReader zip_reader(xmlPath);
        if (zip_reader.exists())
        {
            foreach (QZipReader::FileInfo info, zip_reader.fileInfoList())
            {
                if (info.isFile) // CHECK: Нужна ли эта проверка?
                {
                    QFileInfo currFileInfo(info.filePath);
                    if (currFileInfo.suffix() == "xml")
                    {
                        QByteArray fileData = zip_reader.fileData(info.filePath); // ?
                        QString fileDataStr(fileData);
                        QString fileName = getFileNameWithoutPath(QFile(xmlPath).fileName());
                        query.bindVariable("path", QVariant(fileName));
                        query.setFocus(fileDataStr);
                        query.setQuery(&xslFile);
                        query.evaluateTo(&out);
                    }
                }
            }
        }
    }
    else if (xmlFileInfo.suffix() == "xml")
    {
        QFile xmlFile(xmlPath);

        if (!xmlFile.open(QIODevice::ReadOnly))
        {
            return 1;
        }

        QString fileName = getFileNameWithoutPath(xmlFile.fileName());
        query.bindVariable("path", QVariant(fileName));
        query.setFocus(&xmlFile);
        query.setQuery(&xslFile);
        query.evaluateTo(&out);
        xmlFile.close();
    }
    else
    {
        return 1;
    }

    xslFile.close();
    std::cout << out.toStdString();

    return 0;
}
