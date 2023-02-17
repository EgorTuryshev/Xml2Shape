#ifndef APPCORE_H
#define APPCORE_H

#include "io_shape.h"
#include "loggingcategories.h"
#include "xslt_processor.h"
#include <QObject>
#include <QProcess>
#include <QQmlContext>
#include <QQmlProperty>
#include <QQuickView>
#include <QQuickItem>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include <QQmlContext>

enum CheckState
{
    Unchecked,
    PartiallyChecked,
    Checked
};

class Appcore : public QObject
{
    Q_OBJECT
public:
    explicit Appcore(QObject * parent = nullptr);
    Q_INVOKABLE void test();
    Q_INVOKABLE void openLog();
    Q_INVOKABLE void clearLog();
    Q_INVOKABLE void autoClearingLogChanged(CheckState newState);
};

#endif // APPCORE_H
