#ifndef PERSONALINFOSTRUCT_H
#define PERSONALINFOSTRUCT_H

#include <QString>
#include <QDate>
#include <QByteArray>
#include <QPixmap>
class PersonalInfoStruct
{
public:
	PersonalInfoStruct();
	PersonalInfoStruct(
			int id,
			QString name,
			QString mark,
			QString sex,
			QDate birthday,
			QString mail,
			QString mood,
			QByteArray headImageByte);
	//这是重载运算符
	bool operator ==(PersonalInfoStruct &another);
	PersonalInfoStruct& operator =(PersonalInfoStruct &another);

	qint32 userID;
	QString nickname;
	QString mark;
	QString sex;
	QDate birthday;
	QString mail;
	QString mood;
	QByteArray headImageByte;
	QPixmap headImagePixmap;
};

#endif // PERSONALINFOSTRUCT_H
