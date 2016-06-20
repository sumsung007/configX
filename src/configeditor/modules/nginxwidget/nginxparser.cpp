#include "nginxparser.hpp"

#include "../../networksession.hpp"

#include <QtCore/QDebug>

NginxParser::NginxParser(NetworkSession *session):
    _session(session)
{
    qDebug() << "Create new NginxParser instance";
}

bool NginxParser::parse(QString path, ConfigSection *parent)
{
    qDebug() << "Parse nginx config" << path;

    QString content;
    if (!this->_session->readFileContents(path, &content)) {
        qCritical("Could not read file %s", path.toUtf8().constData());
        return false;
    }

    if (parent == Q_NULLPTR)
        parent = new ConfigSection("", QVariant());

    QString name;
    QString value;
    bool isName = true;

    for (int i = 0; i < content.length(); i++) {
        switch (content.at(i).unicode()) {
            case 59: // ;
                // End of element
                parent->addElement(name.trimmed(), value.trimmed());
                name = "";
                value = "";
                isName = true;
                break;
            case 123: // {
                // Beginning of new section
                parent = parent->addSection(name.trimmed(), value.trimmed());
                name = "";
                value = "";
                isName = true;
                break;
            case 125: // {
                // End of section
                parent = parent->parent();
                name = "";
                value = "";
                isName = true;
                break;
            case 32: // Space
                if (isName && name.length() > 0)
                    isName = false;

                // Space belongs to value
                value.append(content.at(i));
                break;
            default:
                if (isName)
                    name.append(content.at(i));
                else
                    value.append(content.at(i));
        }
    }

    return true;
}
