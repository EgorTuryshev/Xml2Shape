#ifndef XSLT_PROCESSOR_H
#define XSLT_PROCESSOR_H

#include <QString>

class xslt_processor
{
private:
    static QString cwd; // current working directory

    xslt_processor();
public:
    static QString processXSLT(QString xmlPath, QString xsltPath);
    static void setcwd(QString path);
};

#endif // XSLT_PROCESSOR_H
