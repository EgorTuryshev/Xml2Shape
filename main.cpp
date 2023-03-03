#include "appcore.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <iostream>
#include <QDateTime>
#include <QString>
#include <QStringListModel>
#include <model_generator.h>
#include <fs_category.h>
#include <QFile>
#include <model_generator.h>
#include <loggingcategories.h>

QScopedPointer<QFile>   m_logFile;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if (QString::compare(context.category, "ClearLog") == 0)
    {
        m_logFile->remove();
        QFile log("logFile.txt");
        log.resize(0);
        m_logFile.data()->open(QFile::Append | QFile::Text);
    }
    QTextStream out(m_logFile.data());
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
    switch (type)
    {
    case QtInfoMsg:     out << "INF "; break;
    case QtDebugMsg:    out << "DBG "; break;
    case QtWarningMsg:  out << "WRN "; break;
    case QtCriticalMsg: out << "CRT "; break;
    case QtFatalMsg:    out << "FTL "; break;
    }
    out << context.category << ": "
        << msg << Qt::endl;
    out.flush();
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QStringListModel model_categories, model_xslts;
    Appcore appcore(&model_categories);
    Model_Generator generator(&model_xslts, &appcore);
    const QUrl url(u"qrc:/Xml2Shape/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    app.setOrganizationName("TFI");
    app.setApplicationName("Xml2Shape");

    m_logFile.reset(new QFile("logFile.txt"));
    m_logFile.data()->open(QFile::Append | QFile::Text);
    qInstallMessageHandler(messageHandler);

    appcore.refreshCategories();

    /*appcore.ReadCategories();

    QStringList list;
    foreach (fs_category category, Appcore::Get_Categories()) {
        list << category.GetName();
    }
    //list << "one" << "two" << "three" << "four";
    model_categories.setStringList(list);*/

    engine.rootContext()->setContextProperty("appcore", &appcore);
    engine.rootContext()->setContextProperty("model_categories", &model_categories);
    engine.rootContext()->setContextProperty("model_xslts", &model_xslts);
    engine.rootContext()->setContextProperty("generator", &generator);

    engine.load(url); 
    return app.exec();
}
