#ifndef GROUPMEMBERINFOSTRUCT_H
#define GROUPMEMBERINFOSTRUCT_H

#include <QString>

class GroupMemberInfoStruct
{
public:
	GroupMemberInfoStruct(qint32 id=0,QString name=QString(),QString mark=QString());
	//这些是成员
	qint32 userID;//用户ID
	QString nickname;//在群里的昵称(一个人可能所属多个群,可能在不同的群会有不同的昵称)
	QString remark;//在群里的个人说明等等信息暂时先放此处
};

#endif // GROUPMEMBERINFOSTRUCT_H
