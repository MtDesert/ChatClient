#include "XmlWriter.h"
#include "Cipher.h"
#include <QDebug>
#include <QStringList>
#include <QByteArray>
#include <QBuffer>
#include <QDir>
XmlWriter::XmlWriter()
{
    writer.setAutoFormatting(true);
}

void XmlWriter::writeInfo(const QString &filePath, const QString &fileName, const QStringList &value)
{
    QString tempPath = filePath + "/" + fileName;
    qDebug()<<"tempPath"<<tempPath<<endl;
    QFile file(tempPath);

    if("personal.xml" == fileName)
    {//这个模式对应着写入person
        if(file.open(QFile::WriteOnly))
        {
            //准备缓冲区,以保存待加密的数据
            QByteArray array;//用于保存数据
            QBuffer buffer(&array);//将容器与缓冲区关联起来
            buffer.open(QBuffer::WriteOnly);//打开缓冲区
            writer.setDevice(&buffer);//准备保存数据
            //以下是写入数据的代码
            writer.writeStartDocument("1.0");
            writer.writeStartElement("note");
            writer.writeTextElement("name", value.value(0));
            writer.writeTextElement("auto", value.value(1));
            writer.writeTextElement("remember", value.value(2));
            writer.writeTextElement("passwd", value.value(3));
            writer.writeTextElement("state", value.value(4));
            writer.writeEndElement();

            //写入完毕,开始加密
            cipher_Encryption(array,"damo");
            //写入文件
            file.seek(0);
            file.write(array);
            buffer.close();
            file.close();
        }
    }
    else if("friend.xml" == fileName)
    {//
        QDir dir(filePath);
        if(!dir.exists(fileName))
        {//如果文件不存在则创建这个
            if(file.open(QFile::WriteOnly))
            {/////////这里正好空出一个位置出来,以后就放自己在这里
                writer.setDevice(&file);//准备保存数据
                //以下是写入数据的代码
                writer.writeStartDocument("1.0");
                writer.writeStartElement("FriendNote");
                //创建的时候只有开始标志没有结束标志;
                //要不然出现多棵树就解析不了
                writer.writeStartElement("Friend");
                writer.writeEndElement();
            }
            file.close();
        }
        else
        {
            if(file.open(QFile::Append))
            {
                writer.setDevice(&file);//准备保存数据
                //以下是写入数据的代码
                //          writer.writeStartDocument("1.0");
                writer.writeStartElement("Friend");
                writer.writeTextElement("id", value.value(0));
                writer.writeTextElement("nickName", value.value(1));
                writer.writeTextElement("mood", value.value(2));
                writer.writeTextElement("mail", value.value(3));
                writer.writeTextElement("sex", value.value(4));
                writer.writeTextElement("birthday", value.value(5));
                writer.writeTextElement("imagePath", value.value(6));
                writer.writeEndElement();
            }
            file.close();
        }
    }
    else
    {
        qDebug()<<"error !! your no set open mode!"<<endl;
    }
}

