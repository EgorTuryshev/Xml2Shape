#include "geometry.h"

int Geometry::SHPId = 0;
int Geometry::SHPtype = -1;
SHPHandle Geometry::shp;
DBFHandle Geometry::dbf;
QVector<QString> Geometry::DBFFields;
const char* Geometry::SHPpath = "UNKNOWN";
bool Geometry::isSmartReverseEnabled = false;

Geometry::Geometry(int v_SHPtype)
{
    if(SHPtype == -1)
    {
        SHPtype = v_SHPtype;
        shp = SHPCreate(SHPpath, SHPtype);
        dbf = DBFCreate(SHPpath);
    }
    else
    {
        if(v_SHPtype != SHPtype)
        {
            qCritical(logCritical()) << "Записываемый тип геометрических данных не соответствует текущему" << "\nИспользуйте Reset";
        }
    }
    currentId = ++SHPId;
}
QVector<double> Geometry::GetXs()
{
    return Xs;
}
QVector<double> Geometry::GetYs()
{
    return Ys;
}
void Geometry::XPush(double x)
{
    Xs.push_back(x);
}
void Geometry::YPush(double y)
{
    Ys.push_back(y);
}
void Geometry::PointPush(double x, double y)
{
    XPush(x);
    YPush(y);
    nVerts++;
}
void Geometry::AddAttribute(DBFFieldType type, QString field, QString value)
{
    if(!DBFFields.contains(field))
    {
        if(type == FTDouble)
        {
            if(DBFAddField(dbf, &field.toStdString()[0], type, 11, 3) == -1)
            {
                qCritical(logCritical()) << "Не удалось создать поле FTDouble" << " со значением " << value;
            }
        }
        else
        {
            if(DBFAddField(dbf, &field.toStdString()[0], type, 11, 0) == -1)
            {
                qCritical(logCritical()) << "Не удалось создать поле " << type << " со значением " << value;
            }
        }

        DBFFields.push_back(field);
    }

    GATS.push_back(Geometry_Attribute (type, DBFFields.indexOf(field), value));
}
void Geometry::StartSubpart()
{
    if (nParts > 0) SmartReverse();
    nParts++;
    IteratorsOfVerts.push_back(nVerts);
}
void Geometry::WriteToSHP(SHPHandle shp)
{
    SHPWriteObject(shp, -1, SHPCreateObject(SHPtype, currentId, nParts, &IteratorsOfVerts[0], NULL, nVerts, &Xs[0], &Ys[0], NULL, NULL));
}
void Geometry::WriteToDBF(DBFHandle dbf)
{
    for (int i = 0; i < GATS.count(); i++)
    {
        Geometry_Attribute currentGAT = GATS[i];
        qDebug(logDebug()) << currentId << " " << currentGAT.GetField() << " " << &currentGAT.GetValue().toStdString()[0];
        switch(currentGAT.GetType())
        {
            case FTString:
            if (DBFWriteStringAttribute(dbf, currentId - 1, currentGAT.GetField(), &currentGAT.GetValue().toStdString()[0] ) == false)
            {
               qCritical(logCritical()) << "Не удалось записать атрибут FTString по id " << currentId - 1;
            }
            break;
            case FTDouble:
            if (DBFWriteDoubleAttribute(dbf, currentId - 1, currentGAT.GetField(), currentGAT.GetValue().toDouble() ) == false)
            {
                qCritical(logCritical()) << "Не удалось записать атрибут FTDouble по id " << currentId - 1;
            }
            break;
            case FTInteger:
            if (DBFWriteIntegerAttribute(dbf, currentId - 1, currentGAT.GetField(), currentGAT.GetValue().toInt() ) == false)
            {
                qCritical(logCritical()) << "Не удалось записать атрибут FTInteger по id " << currentId - 1;
            }
            break;

            default: qCritical(logCritical()) << "Тип поля не распознан";
        }
    }

}
void Geometry::SetShapeFile(const char* path)
{
    ResetShapeFile();
    SHPpath = path;
    qDebug(logDebug()) << SHPpath;
}
void Geometry::SaveShapeFile()
{
    try
    {
        SHPClose(shp);
        DBFClose(dbf);
    } catch (...)
    {
        qCritical(logCritical()) << "Запись в файл не удалась";
    }

}
void Geometry::WriteToShapeFile()
{
    try
    {
        WriteToSHP(shp);
        WriteToDBF(dbf);
    } catch (...)
    {
        qCritical(logCritical()) << "Запись в формат не удалась";
    }

}
void Geometry::ResetShapeFile()
{
    DBFFields.clear();
    SHPId = 0;
    SHPtype = -1;
}
void Geometry::StartHole()
{
    this->isHoleOnGoing = true;
    StartSubpart();
}
void Geometry::EndHole()
{
    SmartReverse();
    this->isHoleOnGoing = false;
}
bool Geometry::IsClockWise(QVector<double> icw_Xs, QVector<double> icw_Ys)
{
    double s = 0;

    for(int i = 0; i < icw_Xs.count() - 1; i++)
    {
        s += icw_Xs.at(i) * icw_Ys.at(i + 1) - icw_Ys.at(i) * icw_Xs.at(i + 1);
    }

    return s < 0 ? true : false;
}
void Geometry::Reverse()
{
    QVector<double> buffered_Xs = this->GetXs().mid(IteratorsOfVerts.last());
    QVector<double> buffered_Ys = this->GetYs().mid(IteratorsOfVerts.last());

    QVector<double> permutated_Xs;
    QVector<double> permutated_Ys;

    for(int i = buffered_Xs.count() - 2; i >= 0; i--)
    {
        permutated_Xs.push_back(buffered_Xs.at(i));
        permutated_Ys.push_back(buffered_Ys.at(i));
    }
    permutated_Xs.push_back(permutated_Xs.first());
    permutated_Ys.push_back(permutated_Ys.first());

    buffered_Xs.clear();
    buffered_Ys.clear();

    qDebug(logDebug()) << permutated_Xs;
    int j = 0;
    for(int i = IteratorsOfVerts.last(); i < Xs.count(); i++)
    {
        Xs[i] = permutated_Xs.at(j);
        Ys[i] = permutated_Ys.at(j);
        j++;
    }

    permutated_Xs.clear();
    permutated_Ys.clear();
}
void Geometry::Reverse(QVector<double> &buffered_Xs, QVector<double> &buffered_Ys)
{
    QVector<double> permutated_Xs;
    QVector<double> permutated_Ys;

    for(int i = buffered_Xs.count() - 2; i >= 0; i--)
    {
        permutated_Xs.push_back(buffered_Xs.at(i));
        permutated_Ys.push_back(buffered_Ys.at(i));
    }
    permutated_Xs.push_back(permutated_Xs.first());
    permutated_Ys.push_back(permutated_Ys.first());

    buffered_Xs.clear();
    buffered_Ys.clear();

    qDebug(logDebug()) << permutated_Xs;
    int j = 0;
    for(int i = IteratorsOfVerts.last(); i < Xs.count(); i++)
    {
        Xs[i] = permutated_Xs.at(j);
        Ys[i] = permutated_Ys.at(j);
        j++;
    }

    permutated_Xs.clear();
    permutated_Ys.clear();
}
void Geometry::SmartReverse()
{
    if(isSmartReverseEnabled)
    {
        QVector<double> buffered_Xs = this->GetXs().mid(IteratorsOfVerts.last());
        QVector<double> buffered_Ys = this->GetYs().mid(IteratorsOfVerts.last());

        if((IsClockWise(buffered_Xs, buffered_Ys) && isHoleOnGoing) || (!IsClockWise(buffered_Xs, buffered_Ys) && !isHoleOnGoing))
        {
            Reverse(buffered_Xs, buffered_Ys);
        }
    }
}
void Geometry::SetSmartReverse(const bool isEnabled)
{
    isSmartReverseEnabled = isEnabled;
}
