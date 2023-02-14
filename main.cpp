#include "appcore.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include<QFile>
#include<iostream>
#include<QDateTime>
#include "tester.h"

QScopedPointer<QFile>   m_logFile;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
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
    const QUrl url(u"qrc:/Xml2Shape/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

       m_logFile.reset(new QFile("logFile.txt"));
       m_logFile.data()->open(QFile::Append | QFile::Text);
       qInstallMessageHandler(messageHandler);

    Appcore appcore;
    engine.rootContext()->setContextProperty("appcore", &appcore);

    engine.load(url); 
    return app.exec();
}
