#ifndef CONFIGSECTION_HPP
#define CONFIGSECTION_HPP

#include "configelement.hpp"

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QDebug>

class ConfigSection
{
public:
    ConfigSection(QString name, QVariant value, ConfigSection *parent = Q_NULLPTR);
    ~ConfigSection();

    void addElement(const QString name, const QVariant value);
    ConfigSection *addSection(const QString name, const QVariant value);

    ConfigSection *parent();

    QString name() const;
    QVariant value() const;
    QList<ConfigSection *> sections() const;
    QList<ConfigElement *> elements() const;

private:
    QString _name;
    QVariant _value;
    ConfigSection *_parent;

    QList<ConfigSection*> _sections;
    QList<ConfigElement*> _elements;
};

QDebug operator<< (QDebug d, const ConfigSection &model);

#endif // CONFIGSECTION_HPP
