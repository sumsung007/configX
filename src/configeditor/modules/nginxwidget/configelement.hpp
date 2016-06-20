#ifndef CONFIGELEMENT_HPP
#define CONFIGELEMENT_HPP

#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QDebug>

class ConfigElement
{
public:
    ConfigElement(QString name = "", QVariant value = "");

    QString getName() const;
    QVariant getValue() const;

protected:
    QString name;
    QVariant value;
};

QDebug operator<< (QDebug d, const ConfigElement &model);

#endif // CONFIGELEMENT_HPP
