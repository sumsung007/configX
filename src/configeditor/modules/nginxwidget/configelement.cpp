#include "configelement.hpp"

ConfigElement::ConfigElement(QString name, QVariant value):
    name(name),
    value(value)
{
    //
}

QString ConfigElement::getName() const
{
    return this->name;
}

QVariant ConfigElement::getValue() const
{
    return this->value;
}

QDebug operator<< (QDebug d, const ConfigElement &model) {
    d << model.getName() << model.getValue() << ";";
    return d;
}
