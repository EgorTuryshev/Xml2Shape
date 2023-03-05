#ifndef XSLT_PROCESSOR_H
#define XSLT_PROCESSOR_H

#include <QString>
#include "loggingcategories.h"

class xslt_processor
{
private:
    static QString cwd; // current working directory

    xslt_processor();
public:
    static QString processXSLT(QString xmlPath, QString xsltPath);
    static QString processXSLT_data(QString xmlFileName, QString xmlData, QString xsltPath);
    static void setcwd(QString path);
};

#endif // XSLT_PROCESSOR_H
