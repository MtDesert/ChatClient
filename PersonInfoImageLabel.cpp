#include "PersonInfoImageLabel.h"
#include <QDropEvent>
#include <QMouseEvent>
PersonInfoImageLabel::PersonInfoImageLabel(QWidget *parent) :
    ImageLabel(parent)
{

}

void PersonInfoImageLabel::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{//当鼠标单击时,就发出通知,要弹出个人信息窗口
		emit signalShowPersonInfoDlg();
	}
	else
	{
		ImageLabel::mousePressEvent(event);
	}
}

void PersonInfoImageLabel::dropEvent(QDropEvent *event)
{
	ImageLabel::dropEvent(event);		//主要目的不是为了完全改写基类的功能,而是想在本功能之上再添加一些东西
	emit signalUpdataPersonInforToServer();
}


