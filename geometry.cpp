#include "geometry.h"

int Geometry::SHPId = 0;
int Geometry::SHPtype = -1;
SHPHandle Geometry::shp;
DBFHandle Geometry::dbf;
QVector<QString> Geometry::DBFFields;
const char* Geometry::SHPpath = "UNKNOWN";

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
    if(isHoleOnGoing)
    {
        this->hole_buffer_Xs.push_back(x);
        this->hole_buffer_Ys.push_back(y);
    }
    else
    {
        XPush(x);
        YPush(y);
        nVerts++;
    }
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
            if(DBFWriteStringAttribute(dbf, currentId - 1, currentGAT.GetField(), &currentGAT.GetValue().toStdString()[0] ) == 0)
            {
               qCritical(logCritical()) << "Не удалось записать атрибут FTString по id " << currentId - 1;
            }
            break;
            case FTDouble:
            if(DBFWriteDoubleAttribute(dbf, currentId - 1, currentGAT.GetField(), currentGAT.GetValue().toDouble() ) == 0)
            {
                qCritical(logCritical()) << "Не удалось записать атрибут FTDouble по id " << currentId - 1;
            }
            break;
            case FTInteger:
            if(DBFWriteIntegerAttribute(dbf, currentId - 1, currentGAT.GetField(), currentGAT.GetValue().toInt() ) == 0)
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
}
void Geometry::SaveShapeFile()
{
    try
    {
        SHPClose(shp);
        DBFClose(dbf);
    } catch(...)
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
}
void Geometry::EndHole()
{
    this->isHoleOnGoing = false;

    for(int i = hole_buffer_Xs.count() - 2; i >= 0; i--)
    {
        PointPush(hole_buffer_Xs.at(i), hole_buffer_Ys.at(i));
    }
    PointPush(hole_buffer_Xs.first(), hole_buffer_Ys.first());

    hole_buffer_Xs.clear();
    hole_buffer_Ys.clear();
}
