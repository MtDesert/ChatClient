#include "Widget_Name_Mood.h"

Widget_Name_Mood::Widget_Name_Mood(QWidget *parent, const QString name, const QString mark, const QString mood) :
QWidget(parent), strName(name), strMark(mark),strMood(mood)
{
	setupUi(this);
	refresh();
	label->adjustSize();
	label_2->adjustSize();
	this->adjustSize();
	label_2->setToolTip(strMood);
}
//这是大漠添加的刷新功能
void Widget_Name_Mood::refresh()
{
	if(strMark.length()>0)
	{
		label->setText(strMark);
	}
	else
	{
		label->setText(strName);
	}
	label_2->setText(strMood);
}