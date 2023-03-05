#include <QtGui/private/qzipreader_p.h>
#include <QtConcurrent>
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

void Appcore::test(QString xmlFilePath, QString xslFilePath, QString targetPath) // TO-DO: Переименовать метод
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

    QFileInfo xmlFileInfo(xmlFilePath);
    if (xmlFileInfo.suffix() == "zip")
    {
        QZipReader zip_reader(xmlFilePath);
        if (zip_reader.exists())
        {
            // Ниже представлен медленный способ
            /*foreach (QZipReader::FileInfo info, zip_reader.fileInfoList())
            {
                if (info.isFile) // CHECK: Нужна ли эта проверка?
                {
                    QFileInfo currFileInfo(info.filePath);
                    // CHECK: Нужно ли вообще проверять каждый файл по отдельности, или лучше сразу использовать метод
                    // .extractAll()?
                    if (currFileInfo.suffix() == "xml")
                    {
                        QByteArray fileData = zip_reader.fileData(info.filePath); // ?
                        qDebug() << "Считаны данные файла";
                        QString fileDataStr(fileData);
                        qDebug() << "Данные файла переведены в строку";
                        QFile tempFile("./temp/" + currFileInfo.fileName()); // ?

                        if (tempFile.open(QIODevice::WriteOnly | QIODevice::Text))
                        {
                            QTextStream writeStream(&tempFile);
                            writeStream << fileDataStr; // CHECK: Можно ли сразу записывать QByteArray?
                        }
                        tempFile.close();
                        qDebug() << "Данные файла записаны во временный файл";

                        QFileInfo tempFileInfo(tempFile);
                        this->test(tempFileInfo.absoluteFilePath(), xslFilePath, targetPath);

                        tempFile.remove();
                        qDebug() << "Временный файл удален";
                    }
                }
            }*/

            /*zip_reader.extractAll("./temp");
            QDir tempDir("./temp");
            tempDir.setFilter(QDir::Files);
            QFileInfoList files(tempDir.entryInfoList());
            foreach (QFileInfo fileInfo, files)
            {
                if (fileInfo.suffix() == "xml")
                {
                    this->test(fileInfo.absoluteFilePath(), xslFilePath, targetPath);
                }

                QFile::remove(fileInfo.absoluteFilePath());
                //std::filesystem::remove(fileInfo.absoluteFilePath().toStdString());
            }*/

            foreach (QZipReader::FileInfo info, zip_reader.fileInfoList())
            {
                if (info.isFile) // CHECK: Нужна ли эта проверка?
                {
                    QFileInfo currFileInfo(info.filePath);
                    // CHECK: Нужно ли вообще проверять каждый файл по отдельности, или лучше сразу использовать метод
                    // .extractAll()?
                    if (currFileInfo.suffix() == "xml")
                    {
                        QByteArray fileData = zip_reader.fileData(info.filePath); // ?
                        qDebug() << "Считаны данные файла";
                        QString fileDataStr(fileData);
                        qDebug() << "Данные файла переведены в строку";
                        xslt_processor::setcwd("");
                        QString processedXML_str = xslt_processor::processXSLT_data(QFileInfo(xmlFilePath).completeBaseName(),
                                                                                    fileDataStr,
                                                                                    xslFilePath);
                        QString filePath = targetPath + "/" + currFileInfo.completeBaseName();
                        xml_parser::setXML(processedXML_str);
                        QString featureType = xml_parser::readFeatureType();
                        xml_header header = xml_parser::readTypeHeader();
                        QVector<Feature> features = xml_parser::readFeautures();

                        IO_Shape s;
                        s.WriteShape(featureType, header, features, filePath, this->isInvertXY, this->isAutoDirtyFix);
                    }
                }
            }
        }
    }
    else
    {
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

QString Appcore::getCurrentXSLTPath() // TO-DO: добавить проверку на наличие XSL-файла (?)
{
    return removeExt(this->categories.at(this->getCombo1_Index()).GetPath() + categories.at(this->getCombo1_Index()).GetXslts().at(this->getCombo2_Index()).GetRName()) + ".xsl"; // TO-DO: оптимизировать
}

void Appcore::invertXYChanged(bool isInvertXY)
{
    this->isInvertXY = isInvertXY;
}

void Appcore::autoDirtyFixChanged(bool isAutoDirtyFix)
{
    this->isAutoDirtyFix = isAutoDirtyFix;
}

