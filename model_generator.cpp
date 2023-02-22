#include "model_generator.h"

Model_Generator::Model_Generator(QStringListModel * model): m_model(model) {}
void Model_Generator::generate(const QVariant & val)
{
    QStringList list;
    for (int i = 1; i <= 3; ++i)
    {
        list << QString("%1:%2").arg(val.toString()).arg(i);
    }
    m_model->setStringList(list);
}

