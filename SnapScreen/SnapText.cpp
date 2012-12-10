#include <QtGui>
#include <QDebug>
#include "SnapText.h"

SnapText::SnapText(QWidget *parent) :
QTextEdit(parent)
{
	setWindowFlags(Qt::ToolTip);
	setWindowOpacity(0);
	resize(0,0);
	setVisible(true);
	//不显示滚动条
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//让文字透明
}
SnapText::~SnapText()
{
	qDebug()<<"SnapText::~SnapText()";
}

void SnapText::focusOutEvent(QFocusEvent *event)
{
	if(toPlainText()!=QString())
	{
		qDebug()<<"emit signalFinishTyped();";
		emit signalFinishTyped();
	}
}