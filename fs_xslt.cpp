#include "fs_xslt.h"

fs_xslt::fs_xslt(QString  pname, QString  rname, QString  description)
{
    this->_propname = pname;
    this->_realname = rname;
    this->_description = description;
}
QString  fs_xslt::GetPName()
{
    return this->_propname;
}
QString  fs_xslt::GetRName()
{
    return this->_realname;
}
QString  fs_xslt::GetDesc()
{
    return this->_description;
}
