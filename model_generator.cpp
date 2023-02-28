#include "model_generator.h"

Model_Generator::Model_Generator(QStringListModel * model, Appcore * appcore): m_model(model), m_appcore(appcore) {}
void Model_Generator::generate(const QVariant & val)
{
    QStringList list;

    foreach(fs_xslt xslt, Appcore::Get_Categories().at(m_appcore->getCombo1_Index()).GetXslts())
    {
        list << xslt.GetPName();
        //list << QString("%1:%2").arg(val.toString()).arg(i);
    }
    m_model->setStringList(list);
}

