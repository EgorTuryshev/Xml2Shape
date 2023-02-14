#include "feature.h"

Feature::Feature()
{
    this->attributes = QVector<Attribute>();
    this->shells = Shells();
}

Feature::Feature(QVector<Attribute> attributes, Shells shells)
{
    this->attributes = attributes;
    this->shells = shells;
}
