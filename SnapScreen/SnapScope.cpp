#include "QtGui"
#include "SnapScope.h"

SnapScope::SnapScope(QWidget *parent) :
QWidget(parent)
{
	setWindowFlags(Qt::ToolTip);
	setMouseTracking(true);
	//调节尺寸规格
	resize(100,100);
	snapRect.setSize(QSize(50,50));
}
SnapScope::~SnapScope()
{
	qDebug()<<"SnapScope::~SnapScope()";
}
//这些是事件函数
void SnapScope::mouseMoveEvent(QMouseEvent *mouse)
{
	reLocate(mouse->globalPos());
}
void SnapScope::moveEvent(QMoveEvent *move)
{
	snapRect.moveCenter(QCursor::pos()-ptrImageSource->pos());
	snapPixmap=QPixmap::grabWidget(ptrImageSource,snapRect);
	update();
	raise();
}
void SnapScope::paintEvent(QPaintEvent *paint)
{
	QPainter painter(this);
	painter.drawPixmap(rect(),snapPixmap);
	//画瞄准器
	painter.drawLine(0,49,99,49);
	painter.drawLine(0,50,99,50);
	painter.drawLine(49,0,49,99);
	painter.drawLine(50,0,50,99);
}
//这是重定位函数
void SnapScope::reLocate(QPoint mousePos)
{
	int x,y;
	//定位判定
	if(mousePos.x()>=QApplication::desktop()->width()-width())
	{
		x=mousePos.x()-width();
	}
	else
	{
		x=mousePos.x()+1;
	}
	if(mousePos.y()>=QApplication::desktop()->height()-height())
	{
		y=mousePos.y()-height();
	}
	else
	{
		y=mousePos.y()+1;
	}
	//定位执行
	move(x,y);
}