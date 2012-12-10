#include "MyListView.h"
#include <QMouseEvent>
#include <QDebug>
MyListView::MyListView(QWidget *parent) :
    QListView(parent)
{
}

void MyListView::mouseMoveEvent(QMouseEvent *event)
{
	//qDebug()<<event->x()<<" "<<event->y();
	int x = mapToGlobal(event->globalPos()).x();
	int y = mapToGlobal(event->globalPos()).y() + 1;
	//qDebug()<<x;

	QPoint p(this->pos());
//	qDebug()<< p;
	int x1 = mapToGlobal(p).x();
	int x2 = mapToGlobal(p).x() + this->width();
	int y1 =mapToGlobal(p).y() + this->height();
	if(x <= x1 + OFFSET_VIEW || x >= x2 - OFFSET_VIEW || y >= y1 - OFFSET_VIEW)
	{
		this->hide();
	}
}

void MyListView::initSetting()
{
	this->setMouseTracking(true);
}
