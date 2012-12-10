#ifndef XMLWRITER_H
#define XMLWRITER_H
#include <QXmlStreamWriter>
#include <QFile>
class XmlWriter
{
public:
    XmlWriter();
    void writeInfo(const QString &filePath, const QString &fileName, const QStringList &value);
private:
    QXmlStreamWriter writer;

};

#endif // XMLWRITER_H
