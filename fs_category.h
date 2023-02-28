#ifndef FS_CATEGORY_H
#define FS_CATEGORY_H

#include <string>
#include <QVector>
#include <fs_xslt.h>

class fs_category
{
public:
    fs_category(QString  name, QString  path, QString description);
    QString  GetName() const;
    QString  GetPath() const;
    QString GetDescription() const;
    QVector<fs_xslt> GetXslts() const;
    void PushXslt(fs_xslt xslt);
    void SetXslts(QVector<fs_xslt> xslts);
    void Debug_DisplayCategory();
private:
    QString  _name;
    QString _description;
    QString  _path;
    QVector<fs_xslt> _xslts;
};

#endif // FS_CATEGORY_H
