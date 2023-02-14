#ifndef FS_XSLT_H
#define FS_XSLT_H

#include <string>
#include <QString>

using namespace std;

class fs_xslt
{
public:
    fs_xslt(QString  pname, QString  rname, QString  description);
    QString GetPName();
    QString GetRName();
    QString GetDesc();
private:
    QString  _propname;
    QString  _realname;
    QString  _description;
};

#endif // FS_XSLT_H
