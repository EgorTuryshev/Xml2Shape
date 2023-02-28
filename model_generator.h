#ifndef MODEL_GENERATOR_H
#define MODEL_GENERATOR_H

#include <QObject>
#include <QStringListModel>
#include <fs_category.h>
#include <appcore.h>

class Model_Generator : public QObject
{
    Q_OBJECT
    QStringListModel * m_model;
    Appcore * m_appcore;
public:
    Model_Generator(QStringListModel * model, Appcore * appcore);
    Q_INVOKABLE void generate(const QVariant & val);
};

#endif // MODEL_GENERATOR_H
