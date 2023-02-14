#include "appcore.h"

Appcore::Appcore(QObject *parent) : QObject(parent)
{

}
void Appcore::test(){
    IO_Shape s;
    s.WriteShape();
}
void Appcore::openLog()
{
    QStringList p;
    p << "logFile.txt";
    QProcess::execute("notepad.exe", p);
}
void Appcore::clearLog()
{
    qWarning(clr()) << "Лог был очищен пользователем";
}
