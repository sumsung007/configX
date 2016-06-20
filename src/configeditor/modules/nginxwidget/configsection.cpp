#include "configsection.hpp"

ConfigSection::ConfigSection(QString name, QVariant value, ConfigSection *parent):
    _name(name),
    _value(value),
    _parent(parent)
{
    //
}

ConfigSection::~ConfigSection()
{
    qDeleteAll(this->_sections);
    qDeleteAll(this->_elements);
}

void ConfigSection::addElement(const QString name, const QVariant value)
{
    this->_elements.append(new ConfigElement(name, value));
}

ConfigSection *ConfigSection::addSection(const QString name, const QVariant value)
{
    this->_sections.append(new ConfigSection(name, value, this));

    return this->_sections.last();
}

ConfigSection *ConfigSection::parent()
{
    return this->_parent;
}

QString ConfigSection::name() const
{
    return this->_name;
}

QVariant ConfigSection::value() const
{
    return this->_value;
}

QList<ConfigSection *> ConfigSection::sections() const
{
    return this->_sections;
}

QList<ConfigElement *> ConfigSection::elements() const
{
    return this->_elements;
}

QDebug operator<< (QDebug d, const ConfigSection &model) {
    d << model.name() << model.value() << "{";

    for (int i = 0; i < model.sections().count(); i++) {
        d << model.sections().at(i);
    }

    for (int i = 0; i < model.elements().count(); i++) {
        d << model.elements().at(i);
    }

    return d;
}
