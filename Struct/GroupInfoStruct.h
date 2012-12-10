#ifndef GROUPINFOSTRUCT_H
#define GROUPINFOSTRUCT_H

#include <QString>
#include <QList>
#include <QIcon>
#include "GroupMemberInfoStruct.h"

class GroupInfoStruct
{
public:
	GroupInfoStruct();
	//这些是成员
	qint32 number;//群号
	QString name;//群名称
	QIcon icon;//群图标
	QString notice;//群公告
	qint32 administrator;//群主的号码
	QList<qint32> managerList;//管理员名单
	QList<GroupMemberInfoStruct> memberList;//所有成员
};

#endif // GROUPINFOSTRUCT_H
