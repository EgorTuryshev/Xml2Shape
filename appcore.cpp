#include "appcore.h"

Appcore::Appcore(QObject *parent) : QObject(parent)
{

}
void Appcore::test(){
    IO_Shape s;
    s.WriteShape();
}
