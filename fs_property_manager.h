#ifndef FS_PROPERTY_MANAGER_H
#define FS_PROPERTY_MANAGER_H

#include <QFile>
#include <QString>

class fs_property_manager
{
public:
    static QString GetPropertyValue(QString path, QString pname);
    static QString GetFileName(QString path);
private:
    fs_property_manager();
};

#endif // FS_PROPERTY_MANAGER_H
