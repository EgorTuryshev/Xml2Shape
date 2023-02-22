#ifndef MODEL_GENERATOR_H
#define MODEL_GENERATOR_H

#include <QObject>
#include <QStringListModel>

class Model_Generator : public QObject
{
    Q_OBJECT
    QStringListModel * m_model;
public:
    Model_Generator(QStringListModel * model);
    Q_INVOKABLE void generate(const QVariant & val);
};

#endif // MODEL_GENERATOR_H
