#ifndef FRIENDWIDGETITEM_H
#define FRIENDWIDGETITEM_H

#include <QListWidgetItem>
#include "Widget_Name_Mood.h"
#include "Struct/PersonalInfoStruct.h"
#include "CompleteLineEdit.h"

class FriendInfoDetailDialog;
class FriendWidgetItem:public QListWidgetItem
{
public:
	explicit FriendWidgetItem(PersonalInfoStruct &infor, CompleteLineEdit &completeLineEdit);
	Widget_Name_Mood widgetNameMood;
	PersonalInfoStruct friendInfor;
};

#endif // FRIENDWIDGETITEM_H
