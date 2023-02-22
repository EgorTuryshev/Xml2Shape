#include "fs_category.h"
#include "loggingcategories.h"

fs_category::fs_category(QString  name, QString  path, QString description)
{
    this->_name = name;
    this->_path = path;
    this->_description = description;
}
QString  fs_category::GetName()
{
    return this->_name;
}
QString  fs_category::GetPath()
{
    return this->_path;
}
QVector<fs_xslt> fs_category::GetXslts()
{
    return this->_xslts;
}
void fs_category::PushXslt(fs_xslt xslt)
{
    this->_xslts.push_back(xslt);
}
void fs_category::SetXslts(QVector<fs_xslt> xslts)
{
    this->_xslts = xslts;
}
void fs_category::Debug_DisplayCategory()
{
    qDebug(logDebug()) << "ПУТЬ К КАТЕГОРИИ: " << this->GetPath();
    for (int i = 0; i < this->_xslts.count(); i++)
    {
        qDebug(logDebug()) << i <<" ИМЯ XSLT: "<< this->_xslts[i].GetPName();
        qDebug(logDebug()) << i <<" ОПИСАНИЕ XSLT: "<< this->_xslts[i].GetDesc();
        qDebug(logDebug()) << i <<" FS_ИМЯ: "<< this->_xslts[i].GetRName();
    }
}
