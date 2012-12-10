#ifndef WIDGET_NAME_MOOD_H
#define WIDGET_NAME_MOOD_H

#include "ui_Widget_Name_Mood.h"

class Widget_Name_Mood : public QWidget, public Ui::Widget_Name_Mood
{
	Q_OBJECT

public:
	explicit Widget_Name_Mood(QWidget *parent = 0, const QString name = "", const QString mark="", const QString mood = "");
	QString strName;
	QString strMark;
	QString strMood;
	void refresh();//大漠添加的刷新功能,修改心情和备注时使用
};

#endif // WIDGET_NAME_MOOD_H
