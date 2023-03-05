#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QProcess>
#include <QQmlContext>
#include <QQmlProperty>
#include <QQuickView>
#include <QQuickItem>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "fs_category.h"
#include <loggingcategories.h>
#include <QStringListModel>

class Appcore : public QObject
{
    Q_OBJECT
    QStringListModel * m_model_categories;
public:
    explicit Appcore(QStringListModel * model_categories, QObject * parent = nullptr);
    Q_INVOKABLE void test(QString xmlFilePath = "../Xml2Shape/samples/test.xml",
                          QString xslFilePath = "../Xml2Shape/samples/kpt.xsl",
                          QString targetPath = "./");
    Q_INVOKABLE void refreshCategories();
    Q_INVOKABLE void openLog();
    Q_INVOKABLE void clearLog(bool isAutoClearing = false);
    Q_INVOKABLE int getCombo1_Index();
    Q_INVOKABLE int getCombo2_Index();
    Q_INVOKABLE void setCombo1_Index(QVariant val);
    Q_INVOKABLE void setCombo2_Index(QVariant val);
    Q_INVOKABLE QVariant getCurrentCategoryDescription();
    Q_INVOKABLE QVariant getCurrentXSLTDescription();
    Q_INVOKABLE QVariant getFileName(QVariant val);
    Q_INVOKABLE QString getCurrentXSLTPath();
    Q_INVOKABLE void invertXYChanged(bool isInvertXY);
    Q_INVOKABLE void autoDirtyFixChanged(bool isAutoDirtyFix);
    static Q_INVOKABLE void ReadCategories();
    static QVector<fs_category> Get_Categories();
    static int GetCategoryByName(QVector<fs_category> categories, QString name);
private:
    static QVector<fs_category> categories;
    int SelectedIndex_combo1;
    int SelectedIndex_combo2;
    bool isInvertXY;
    bool isAutoDirtyFix;
};

#endif // APPCORE_H
