#include "geometry.h"
#include "loggingcategories.h"

int Geometry::SHPId = 0;

Geometry::Geometry()
{
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
void Geometry::AddAttribute(int type, int field, QString value)
{
    GATS.push_back(Geometry_Attribute (type, field, value));
}
void Geometry::StartHole()
{
    nParts++;
    IteratorsOfVerts.push_back(nVerts);
}
void Geometry::EndHole()
{
    if (IteratorsOfVerts.last() > nVerts){ qWarning(logWarning()) << "Запись части невозможна, отсутствует целостность вектора вершин"; }
    IteratorsOfVerts.count() < 4 ? qWarning(logWarning()) << "Запись части невозможна, недостаточно вершин части" :
                                                             qDebug(logDebug()) << "Часть была успешно записана";
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
            case FTString: DBFWriteStringAttribute(dbf, currentId - 1, currentGAT.GetField(), &currentGAT.GetValue().toStdString()[0] ); break;
            case FTDouble: DBFWriteDoubleAttribute(dbf, currentId - 1, currentGAT.GetField(), currentGAT.GetValue().toDouble() ); break;
            case FTInteger: DBFWriteIntegerAttribute(dbf, currentId - 1, currentGAT.GetField(), currentGAT.GetValue().toInt() ); break;
            default: qCritical(logCritical()) << "Тип поля не распознан";
        }
    }

}
