#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include "io_shape.h"

class Appcore : public QObject
{
    Q_OBJECT
public:
    explicit Appcore(QObject * parent = nullptr);
    Q_INVOKABLE void test();
};

#endif // APPCORE_H
