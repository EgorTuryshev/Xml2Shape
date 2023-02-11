#ifndef XSLT_PROCESSOR_H
#define XSLT_PROCESSOR_H

#include <QString>

class xslt_processor
{
private:
    QString cwd; // current working directory
public:
    xslt_processor();
    QString processXSLT(QString xmlPath, QString xsltPath);
    void setcwd(QString path);
};

#endif // XSLT_PROCESSOR_H
