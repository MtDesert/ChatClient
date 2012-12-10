#include "XmlReader.h"
#include "Cipher.h"
#include <QFile>
#include <QFileDialog>
#include <QByteArray>
#include <QBuffer>
#include <QDebug>
XmlReader::XmlReader()
{

}

void XmlReader::readNote()
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if("name"==reader.name())
            {
                readName();
            }
            else if("passwd" == reader.name())
            {
                readPasswd();
            }
            else if("state" == reader.name())
            {
                readState();
            }
            else if("remember" == reader.name())
            {
                readRememberPasswd();
            }
            else if("auto" == reader.name())
            {
                readAtuoLogin();
            }
        }
    }
}

void XmlReader::readName()
{
    nameStr = reader.readElementText();
    if(reader.isEndElement())
    {
        reader.readNext();
    }
    m_NameList += nameStr;

}

void XmlReader::readPasswd()
{
    passwdStr = reader.readElementText();
    if(reader.atEnd())
    {
        reader.readNext();
    }

}

void XmlReader::readState()
{
    stateStr = reader.readElementText();
    if(reader.isEndElement())
    {
        reader.readNext();
    }
}

QStringList XmlReader::GetFileNameList()
{
    return m_NameList;
}

void XmlReader::Foreach(const QString &fileName)
{
    QFile file(fileName);
    if(file.open(QFile::ReadOnly))
    {

        //先读入数据,进行解密
        QByteArray array=file.readAll();//读取所有数据
        cipher_Decryption(array,"damo");//解密
        QBuffer buffer(&array);//关联容器与缓冲区
        buffer.open(QBuffer::ReadOnly);//打开缓冲区
        reader.setDevice(&buffer);//关联缓冲区
        //开始读取数据

        while(!reader.atEnd())
        {
            reader.readNext();
            if(reader.isStartElement())
            {
                if("note"==reader.name())
                {
                    readNote();
                }
                //////这里写对好友列表的
                else if("FriendNote" == reader.name())
                {
                    readFriendNote();
                }
            }
        }
        buffer.close();
        file.close();
    }
}

void XmlReader::readRememberPasswd()
{
    rememberPasswdStr = reader.readElementText();
    if(reader.isEndElement())
    {
        reader.readNext();
    }
}

void XmlReader::readAtuoLogin()
{
    atuoLoginStr = reader.readElementText();
    if(reader.isEndElement())
    {
        reader.readNext();
    }
}

QString XmlReader::getName()
{
    return nameStr;
}

QString XmlReader::getpasswd()
{
    return passwdStr;
}

QString XmlReader::getState()
{
    return stateStr;
}

QString XmlReader::getRemeberPasswd()
{
    return rememberPasswdStr;
}

QString XmlReader::getAtuoLogin()
{
    return atuoLoginStr;
}

void XmlReader::readFriendNote()
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if("FriendNote" == reader.name())
            {
                readFriend();
            }
        }
    }
}

void XmlReader::readFriend()
{
    while(!reader.atEnd())
    {
        reader.readNext();
        if(reader.isStartElement())
        {
            if("id" == reader.name())
            {
                readId();
            }
            else if("nickName" == reader.name())
            {
                readNickName();
            }
            else if("mood" == reader.name())
            {
                readMood();
            }
            else if("mail" == reader.name())
            {
                readMail();
            }
            else if("sex" == reader.name())
            {
                readSex();
            }
            else if("birthday" == reader.name())
            {
                readBirthday();
            }
            else if("imagePath" == reader.name())
            {
                readImage();
            }
        }
    }
}

void XmlReader::readId()
{
    id = reader.readElementText();
    if(reader.isEndElement())
    {
        reader.readNext();
    }

    qDebug()<<id<<endl;
}

void XmlReader::readNickName()
{
    nickName = reader.readElementText();
    if(reader.isEndElement())
    {
        reader.readNext();
    }
    qDebug()<<nickName<<endl;
}

void XmlReader::readMood()
{
    mood = reader.readElementText();
    if(reader.isEndElement())
    {
        reader.readNext();
    }
    qDebug()<<mood<<endl;
}

void XmlReader::readMail()
{
    mail = reader.readElementText();
    if(reader.isEndElement())
    {
        reader.readNext();
    }
    qDebug()<<mail<<endl;
}

void XmlReader::readSex()
{
    sex = reader.readElementText();
    if(reader.isEndElement())
    {
        reader.readNext();
    }
    qDebug()<<sex<<endl;
}

void XmlReader::readBirthday()
{
    brithday.fromString(reader.readElementText(), ("yyyyMMdd"));
    if(reader.isEndElement())
    {
        reader.readNext();
    }
    qDebug()<<brithday.toString("yyyyMMdd")<<endl;
}

void XmlReader::readImage()
{
    imagePath = reader.readElementText();
    if(reader.isEndElement())
    {
        reader.readNext();
    }
    qDebug()<<imagePath<<endl;
}
