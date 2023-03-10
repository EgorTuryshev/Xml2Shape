#include "fs_property_manager.h"
#include <QTextStream>
#include <loggingcategories.h>
#include <QFileInfo>

QString fs_property_manager::GetPropertyValue(QString path, QString pname)
{
    QString oppositepname = pname == "Name" ? "Description" : "Name";
    QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug(logDebug()) << "ОШИБКА ЧТЕНИЯ";
            return "";
        }

        QString line = "";
        QString linebuffer = "";
        QTextStream in(&file);

        while (!in.atEnd() && !line.contains(pname + ":")) {
             line = in.readLine();
        }

        while (!in.atEnd()) {
            linebuffer = in.readLine();
            if(!linebuffer.contains(oppositepname + ":"))
            {
                if(linebuffer.contains("#"))
                {
                    linebuffer = linebuffer.mid(0, linebuffer.indexOf("#"));
                }
                line += linebuffer;
            }
            else
            {
                break;
            }
        }

        file.close();
        return line.sliced(line.lastIndexOf(pname + ": ") + pname.length() + 2).trimmed();
}
QString fs_property_manager::GetFileName(QString path)
{
    return QFileInfo(path).fileName();
}
QString fs_property_manager::GetNameProperty(QString path)
{
    return GetPropertyValue(path, "Name");
}
QString fs_property_manager::GetDescProperty(QString path)
{
    return GetPropertyValue(path, "Description");
}
