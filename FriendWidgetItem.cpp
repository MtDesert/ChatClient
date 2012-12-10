#include "FriendWidgetItem.h"
#include <QDebug>
FriendWidgetItem::FriendWidgetItem(PersonalInfoStruct &infor, CompleteLineEdit &completeLineEdit)
{
	friendInfor = infor;//这里不知道会不会出现生命周期的问题
	widgetNameMood.strName=friendInfor.nickname;
	widgetNameMood.strMark=friendInfor.mark;
	widgetNameMood.strMood=friendInfor.mood;
	widgetNameMood.refresh();
	if(friendInfor.mark.length())
	{
		completeLineEdit.setHashData(friendInfor.mark, QIcon(friendInfor.headImagePixmap));
	}
	else
	{
		completeLineEdit.setHashData(friendInfor.nickname, QIcon(friendInfor.headImagePixmap));
	}
	this->setIcon(friendInfor.headImagePixmap);
}