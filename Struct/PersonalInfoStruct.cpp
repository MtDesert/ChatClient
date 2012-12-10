#include "PersonalInfoStruct.h"
#include <QDebug>
#include <QBuffer>
PersonalInfoStruct::PersonalInfoStruct()
{
	mark = "";
}

PersonalInfoStruct::PersonalInfoStruct(qint32 id, QString name, QString mark, QString sex, QDate birthday, QString mail, QString mood, QByteArray headImage)
{
	this->userID = id;
	this->nickname = name;
	this->mark = mark;
	this->sex = sex;
	this->birthday = birthday;
	this->mail = mail;
	this->mood = mood;
	this->headImageByte = headImage;
	this->headImagePixmap.loadFromData(headImageByte);
}

//重载运算
bool PersonalInfoStruct::operator ==(PersonalInfoStruct &another)
{
	if(userID==another.userID&&
			nickname==another.nickname&&
			sex==another.sex&&
			birthday==another.birthday&&
			mail==another.mail&&
			mood==another.mood&&
			headImageByte==another.headImageByte)
	{
		return true;
	}
	else return false;
}

PersonalInfoStruct& PersonalInfoStruct::operator =(PersonalInfoStruct &another)
{
	userID = another.userID;
	mark=another.mark;
	nickname = another.nickname;
	sex = another.sex;
	birthday = another.birthday;
	mail = another.mail;
	mood = another.mood;
	headImageByte = another.headImageByte;
	//headImagePixmap = another.headImagePixmap;
	headImagePixmap.loadFromData(headImageByte);
	return *this;
}