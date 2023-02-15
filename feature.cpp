#include "feature.h"

Feature::Feature()
{
    this->attributes = QVector<Attribute>();
    this->shells = QVector<Shell>();
}

Feature::Feature(QVector<Attribute> attributes, QVector<Shell> shells)
{
    this->attributes = attributes;
    this->shells = shells;
}
