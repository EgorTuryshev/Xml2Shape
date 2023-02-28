#include "appcore.h"
#include "xslt_processor.h"
#include "fs_property_manager.h"
#include "io_shape.h"

QVector<fs_category> Appcore::categories = QVector<fs_category>();

QString removeExt(QString str)
{
    int dotInd = 0;
    for (int i = str.length() - 1; i >= 0; i--)
    {
        if (str[i] == '.')
        {
            dotInd = i;
            break;
        }
    }

    return (str.first(dotInd));
}

int SwitchString(QString str)
{
    if(str == "root.desc")
    {
        return 0;
    }
    else
    {
        return -1;
    }

}

void Appcore::ReadCategories()
{
    QString  lastdir = ".";
    QString itdir = ".";
    QVector <QString> file_paths;
    QVector <fs_category> categories;
    QVector <fs_xslt> category_xslts;

    bool ReadMarker = true;
    bool isOnLastDir = false;

    QDirIterator it(QDir::currentPath() + "/templates", {"*.desc"}, QDir::Files, QDirIterator::Subdirectories);

    if (it.hasNext())
    {
        it.next();
        itdir = QFileInfo(it.filePath()).dir().dirName();
        lastdir = itdir;
    }

    while(ReadMarker || isOnLastDir)
    {
        if(lastdir != itdir || isOnLastDir)
        {
            for(int i = 0; i < file_paths.count(); i++)
            {
                QString filename = fs_property_manager::GetFileName(file_paths[i]);
                QString name = fs_property_manager::GetNameProperty(file_paths[i]);
                QString description = fs_property_manager::GetDescProperty(file_paths[i]);

                switch(SwitchString(filename))
                {
                case 0:
                {
                    categories.push_back(fs_category(name, file_paths[i].mid(0, file_paths[i].lastIndexOf("/") + 1), description));
                    break;
                }
                default:
                {
                    category_xslts.push_back(fs_xslt(name, filename, description));
                    break;
                }
                }
            }

            categories.last().SetXslts(category_xslts);
            //categories.last().Debug_DisplayCategory();

            lastdir = itdir;
            file_paths.clear();
            category_xslts.clear();
            isOnLastDir = false;
        }
        else
        {
            file_paths.push_back(it.filePath());

            if(it.hasNext())
            {
                it.next();
                itdir = QFileInfo(it.filePath()).dir().dirName();
            }
            else
            {
                ReadMarker = false;
                isOnLastDir = true;
            }

        }
    }
    Appcore::categories = categories;
}

Appcore::Appcore(QObject *parent) : QObject(parent) { }

void Appcore::test(QString xmlFilePath, QString xslFilePath, QString targetPath){
    if (xmlFilePath.first(8) == "file:///")
    {
        xmlFilePath = xmlFilePath.remove(0, 8);
    }
    if (xslFilePath.first(8) == "file:///")
    {
        xslFilePath = xslFilePath.remove(0, 8);
    }
    if (targetPath.first(8) == "file:///")
    {
        targetPath = targetPath.remove(0, 8);
    }

    qDebug(logDebug()) << xmlFilePath << " " << xslFilePath;

    QString filePath = targetPath + "/" + QFileInfo(xmlFilePath).completeBaseName(); // TO-DO: Доработать, чтобы имя было уникальным для директории
    qDebug(logDebug()) << filePath;
    xslt_processor::setcwd("");
    QString processedXML_str = xslt_processor::processXSLT(xmlFilePath, xslFilePath);
    xml_parser::setXML(processedXML_str);
    QString featureType = xml_parser::readFeatureType();
    xml_header header = xml_parser::readTypeHeader();
    QVector<Feature> features = xml_parser::readFeautures();

    IO_Shape s;
    s.WriteShape(featureType, header, features, filePath);

}
void Appcore::openLog()
{
    QStringList p;
    p << "logFile.txt";
    QProcess::execute("notepad.exe", p);
}
void Appcore::clearLog(bool isAutoClearing)
{
    if (isAutoClearing) qWarning(clr()) << "Лог был автоматически очищен программой";
    else qWarning(clr()) << "Лог был чищен пользователем";
}
QVector<fs_category> Appcore::Get_Categories()
{
    if(!categories.isEmpty())
    {
        return categories;
    }
    else
    {
        ReadCategories();
        return categories;
    }

}

int Appcore::GetCategoryByName(QVector<fs_category> categories, QString name)
{
    for(int i = 0; i < categories.count(); i++)
    {
        if(categories.at(i).GetName() == name)
        {
            return i;
        }
    }
    return -1;
}

int Appcore::getCombo1_Index()
{
    return this->SelectedIndex_combo1;
}

int Appcore::getCombo2_Index()
{
    return this->SelectedIndex_combo2;
}

void Appcore::setCombo1_Index(QVariant val)
{
    this->SelectedIndex_combo1 = val.toInt();
}

void Appcore::setCombo2_Index(QVariant val)
{
    this->SelectedIndex_combo2 = val.toInt();
}

QVariant Appcore::getCurrentCategoryDescription()
{
    return this->categories.at(this->getCombo1_Index()).GetDescription();
}
QVariant Appcore::getCurrentXSLTDescription()
{
    return this->categories.at(this->getCombo1_Index()).GetXslts().at(this->getCombo2_Index()).GetDesc();
}

QString Appcore::getCurrentXSLTPath()
{
    return this->categories.at(this->getCombo1_Index()).GetPath() + categories.at(this->getCombo1_Index()).GetXslts().at(this->getCombo2_Index()).GetRName();
}

