#include <QCoreApplication>
#include <QXmlQuery>
#include <QFile>
#include <QFileInfo>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc < 3)
    {
        return 1;
    }

    QString out = "";
    QXmlQuery query(QXmlQuery::XSLT20);
    QFile xmlFile(argv[1]);
    QFile xslFile(argv[2]);

    xmlFile.open(QIODevice::ReadOnly);
    xslFile.open(QIODevice::ReadOnly);

    query.bindVariable("path", QVariant(QFileInfo(xmlFile).fileName()));
    query.setFocus(&xmlFile);
    query.setQuery(&xslFile);
    query.evaluateTo(&out);
    xmlFile.close();
    xslFile.close();

    std::cout << out.toStdString();
}
