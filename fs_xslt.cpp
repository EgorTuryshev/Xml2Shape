#include "fs_xslt.h"

fs_xslt::fs_xslt(QString  pname, QString  rname, QString  description)
{
    this->_propname = pname;
    this->_realname = rname;
    this->_description = description;
}
QString  fs_xslt::GetPName() const
{
    return this->_propname;
}
QString  fs_xslt::GetRName() const
{
    return this->_realname;
}
QString  fs_xslt::GetDesc() const
{
    return this->_description;
}
