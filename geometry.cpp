#include "geometry.h"

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
    Xs.push_back(y);
}
void Geometry::PointPush(double x, double y)
{
    XPush(x);
    YPush(y);
    nVerts++;
}
void Geometry::AddAttribute(int type, string value)
{
    GATS.push_back(Geometry_Attribute (this->lastAttrId, type, value));
    ++this->lastAttrId;
}
void Geometry::StartHole()
{
    nParts++;
    IteratorsOfVerts.push_back(nVerts + 1);
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
    for(int i = 0; i < GATS.count(); i++)
    {
        Geometry_Attribute currentGAT = GATS[i];
        switch(currentGAT.GetType())
        {
            case FTString: DBFWriteStringAttribute(dbf, currentId, currentGAT.GetField(), &currentGAT.GetValue()[0] );
            case FTDouble: DBFWriteDoubleAttribute(dbf, currentId, currentGAT.GetField(), stod(currentGAT.GetValue()) );
            case FTInteger: DBFWriteIntegerAttribute(dbf, currentId, currentGAT.GetField(), stoi(currentGAT.GetValue()) );
            default: qCritical(logCritical()) << "Тип поля не распознан";
        }
    }

}
