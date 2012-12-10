#include "LocalDataBase.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "Cipher.h"
#include "Struct/PersonalInfoStruct.h"

//添加好友记录
bool localDatabase_insertFriend(QSqlDatabase& database,PersonalInfoStruct& info)
{
	qDebug()<<"localDatabase_insertFriend()";
	if(!database.isOpen())
	{
		qDebug()<<"database is not open";
		return false;
	}
	QSqlQuery query(database);
	if(!query.prepare("insert into friend_infor values(?,?,?,?,?,?,?,?)"))
	{
		qDebug()<<"prepare()==false";
		return false;
	}
	query.bindValue(0,info.userID);
	query.bindValue(1,info.mark);
	query.bindValue(2,info.nickname);
	query.bindValue(3,info.sex);
	query.bindValue(4,info.birthday);
	query.bindValue(5,info.mail);
	query.bindValue(6,info.mood);
	query.bindValue(7,info.headImageByte);
	if(query.exec())
	{
		return true;
	}
	else
	{
		qDebug()<<"exec()==false:"<<query.lastError().text();
		return false;
	}
}
//更新好友记录
bool localDatabase_updateFriend(QSqlDatabase& database,PersonalInfoStruct& info)
{
	qDebug()<<"localDatabase_updateFriend()";
	if(!database.isOpen())
	{
		qDebug()<<"database is not open";
		return false;
	}
	QSqlQuery query(database);
	if(!query.prepare("update friend_infor set friend_remarks=?,friend_nickname=?,friend_sex=?,friend_birthday=?,friend_mail=?,friend_mood=?,friend_headimage=? where friend_id=?"))
	{
		qDebug()<<"prepare()==false";
		return false;
	}
	query.bindValue(0,info.mark);
	query.bindValue(1,info.nickname);
	query.bindValue(2,info.sex);
	query.bindValue(3,info.birthday);
	query.bindValue(4,info.mail);
	query.bindValue(5,info.mood);
	query.bindValue(6,info.headImageByte);
	query.bindValue(7,info.userID);
	if(query.exec())
	{
		return true;
	}
	else
	{
		qDebug()<<"exec()==false:"<<query.lastError().text();
		return false;
	}
}
//删除好友记录
bool localDatabase_deleteFriend(QSqlDatabase& database,qint32 id)
{
	qDebug()<<"localDatabase_deleteFriend()";
	if(!database.isOpen())
	{
		qDebug()<<"database is not open";
		return false;
	}
	QSqlQuery query(database);
	if(!query.prepare("delete from friend_infor where friend_id=?)"))
	{
		qDebug()<<"prepare()==false";
		return false;
	}
	query.addBindValue(id);
	if(query.exec())
	{
		return true;
	}
	else
	{
		qDebug()<<"exec()==false:"<<query.lastError().text();
		return false;
	}
}
//读取个人信息
bool localDatabase_readUserInfo(QDir dataDir,PersonalInfoStruct& info)
{
	qDebug()<<"localDatabase_readUserInfo()";
	QFile personalDataFile("./user/"+dataDir.dirName()+"/personal.dat");
	if(personalDataFile.open(QIODevice::ReadOnly))
	{
		QByteArray byteData;
		//从文件读
		byteData=personalDataFile.readAll();
		personalDataFile.close();
		//先解
		cipher_BitXor(byteData,"DEMO2048");
		QDataStream streamData(&byteData,QIODevice::ReadOnly);
		//再定义缓
		qint32 tmpInt;
		QString tmpString;
		QByteArray tmpByteArray;
		//读取
		streamData>>tmpInt;info.userID=tmpInt;
		streamData>>tmpString;info.nickname=tmpString;
		streamData>>tmpString;info.sex=tmpString;
		streamData>>tmpString;info.birthday=QDate::fromString(tmpString);
		streamData>>tmpString;info.mail=tmpString;
		streamData>>tmpString;info.mood=tmpString;
		streamData>>tmpByteArray;info.headImageByte=tmpByteArray;
		return true;
	}
	else
	{
		qDebug()<<"read failed";
		return false;
	}
}
//写回个人信息
bool localDatabase_writeUserInfo(QDir dataDir,PersonalInfoStruct& info)
{
	qDebug()<<"localDatabase_writeUserInfo()";
	QFile personalDataFile("./user/"+dataDir.dirName()+"/personal.dat");
	if(personalDataFile.open(QIODevice::WriteOnly))
	{
		QByteArray byteData;
		QDataStream streamData(&byteData,QIODevice::WriteOnly);
		//写入数据
		streamData<<info.userID;
		streamData<<info.nickname;
		streamData<<info.sex;
		streamData<<info.birthday.toString();
		streamData<<info.mail;
		streamData<<info.mood;
		streamData<<info.headImageByte;
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//在这个地方暂时插入一句，用于保存服务器上的头像到本地---这里和大漠的有区别，不是直接存储在文件中
		QPixmap headImageFromServer;
		headImageFromServer.loadFromData(info.headImageByte);
		//		qDebug()<<"myDir:"<<myDir->path();
		headImageFromServer.save(dataDir.path() + "/image", "PNG");
		//headImageFromServer.save(myDir)
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		cipher_BitXor(byteData,"DEMO2048");//加密数据
		personalDataFile.write(byteData);//写入文件
		personalDataFile.close();//关闭文件
		return true;
	}
	else
	{
		qDebug()<<"write failed";
		return false;
	}
}