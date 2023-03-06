#include <QCoreApplication>
#include <QXmlQuery>
#include <QFile>
#include <QSharedMemory>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc < 3)
    {
        return 1;
    }

    QString xmlData;
    QString out = "";
    QXmlQuery query(QXmlQuery::XSLT20);

    QSharedMemory mem;
    mem.setKey("xmlData"); // CHECK: Могут быть проблемы на MacOS, там названия должны быть стандартизированы
    if (!mem.attach()) // Подключаемся к уже выделенной общей памяти
    {
        return 1;
    }
    char *data = (char*)mem.data(); // Указатель на общую память
    if (mem.lock())
    {
        QByteArray xmlDataBytes;
        for (int i = 0; data[i]; i++)
            xmlDataBytes.append(data[i]);
        mem.unlock();
        xmlData = QString(xmlDataBytes); // ?
    }
    mem.detach();

    QString xmlFileName(argv[1]);
    QFile xslFile(argv[2]);

    xslFile.open(QIODevice::ReadOnly);

    query.bindVariable("path", QVariant(xmlFileName));
    query.setFocus(xmlData);
    query.setQuery(&xslFile);
    query.evaluateTo(&out);
    xslFile.close();

    std::cout << out.toStdString();

    return 0;
}
