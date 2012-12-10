#ifndef XMLREADER_H
#define XMLREADER_H

#include <QXmlStreamReader>
#include <QStringList>
#include <QDate>
class QFile;
class XmlReader
{
public:
    XmlReader();
    QStringList GetFileNameList();
    void Foreach(const QString &fileName);

    QString getName();
    QString getpasswd();
    QString getState();
    QString getRemeberPasswd();
    QString getAtuoLogin();

private:
    void readNote();
    void readName();
    void readPasswd();
    void readState();
    void readRememberPasswd();
    void readAtuoLogin();

    void readFriendNote();
    void readFriend();
    void readId();
    void readNickName();
    void readMood();
    void readMail();
    void readSex();
    void readBirthday();
    void readImage();
private:
    QString nameStr;
    QString passwdStr;
    QString stateStr;
    QString rememberPasswdStr;
    QString atuoLoginStr;

    QString id;
    QString nickName;
    QString mood;
    QString mail;
    QString sex;
    QDate brithday;
    QString imagePath;

    QStringList m_NameList;
    QXmlStreamReader reader;
};

#endif // XMLREADER_H
