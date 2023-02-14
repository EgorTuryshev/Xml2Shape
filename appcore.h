#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include "io_shape.h"
#include "loggingcategories.h"
#include <QProcess>
#include <QQmlContext>
#include <QQmlProperty>
#include <QQuickView>
#include <QQuickItem>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include <QQmlContext>

class Appcore : public QObject
{
    Q_OBJECT
public:
    explicit Appcore(QObject * parent = nullptr);
    Q_INVOKABLE void test();
    Q_INVOKABLE void openLog();
    Q_INVOKABLE void clearLog();
};

#endif // APPCORE_H
