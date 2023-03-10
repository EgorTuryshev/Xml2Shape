#include <QtGui/private/qzipreader_p.h>
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

Appcore::Appcore(QStringListModel * model_categories, QObject *parent) : m_model_categories(model_categories), QObject(parent)
{

}

void Appcore::test(QString xmlFilePath, QString xslFilePath, QString targetPath) // TO-DO: ?????????????????????????? ??????????
{
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

    xslt_processor::setcwd("");
    QString processedXML_str = xslt_processor::processXSLT(xmlFilePath, xslFilePath);
    QString filePath = targetPath + "/" + QFileInfo(xmlFilePath).completeBaseName();
    xml_parser::setXML(processedXML_str);
    QString featureType = xml_parser::readFeatureType();
    xml_header header = xml_parser::readTypeHeader();
    QVector<Feature> features = xml_parser::readFeautures();

    IO_Shape s;
    s.WriteShape(featureType, header, features, filePath, this->isInvertXY, this->isAutoDirtyFix);
}

void Appcore::refreshCategories()
{
    this->ReadCategories();

    QStringList list;
    foreach (fs_category category, Appcore::Get_Categories()) {
        list << category.GetName();
    }
    this->m_model_categories->setStringList(list);
}
void Appcore::openLog()
{
    QStringList p;
    p << "logFile.txt";
    QProcess::execute("notepad.exe", p);
}
void Appcore::clearLog(bool isAutoClearing)
{
    if (isAutoClearing) qWarning(clr()) << "?????? ?????? ?????????????????????????? ???????????? ????????????????????";
    else qWarning(clr()) << "?????? ?????? ?????????? ??????????????????????????";
}
QVector<fs_category> Appcore::Get_Categories()
{
    if (!categories.isEmpty())
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

QVariant Appcore::getFileName(QVariant val)
{
    QString path = val.toString();
    path = path.mid(path.lastIndexOf("/") + 1);
    return path;
}

QVariant Appcore::getFilePath(QVariant val)
{
    QString path = val.toString();
    path = path.mid(path.lastIndexOf("///") + 3);
    return path;
}

QString Appcore::getCurrentXSLTPath() // TO-DO: ???????????????? ???????????????? ???? ?????????????? XSL-?????????? (?)
{
    return removeExt(this->categories.at(this->getCombo1_Index()).GetPath() + categories.at(this->getCombo1_Index()).GetXslts().at(this->getCombo2_Index()).GetRName()) + ".xsl"; // TO-DO: ????????????????????????????
}

void Appcore::invertXYChanged(bool isInvertXY)
{
    this->isInvertXY = isInvertXY;
}

void Appcore::autoDirtyFixChanged(bool isAutoDirtyFix)
{
    this->isAutoDirtyFix = isAutoDirtyFix;
}

