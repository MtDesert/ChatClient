#include "MessageRemindForm.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QDebug>
MessageRemindForm::MessageRemindForm(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);
	initSetting();
	initData();
}

void MessageRemindForm::addMessage(int id, QPixmap headimage, QString str, QString chatHtml)
{
	QStandardItem *item = new QStandardItem(QIcon(headimage), str);
	item->setData(id);
	item->setData(chatHtml, Qt::UserRole + 2);
	model->appendRow(item);
	++count;
}
/*
  //这样也被证实是失败的
void MessageRemindForm::deleteObjectLater()
{
	this->deleteLater();
}
*/
void MessageRemindForm::mouseMoveEvent(QMouseEvent *event)
{
//	qDebug()<<"on this widget";
	if(event->buttons() == Qt::LeftButton)
	{
		this->move(event->globalPos() - offset);
		view->move(this->x(), this->y() + this->height());
	}
	else
	{
		QWidget::mouseMoveEvent(event);
	}
	if(view->isHidden())
	{
		view->setMinimumWidth(this->width());
		view->setMaximumWidth(this->width());
		int x = this->x();
		int y = this->y() + this->height() + 1;

		view->move(x, y);
		view->setMouseTracking(true);
		view->setModel(model);
		view->show();
	}
	else
	{
		return;
	}
}

void MessageRemindForm::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		offset = event->globalPos() - frameGeometry().topLeft();
	}
	else
	{
		QWidget::mousePressEvent(event);
	}
}

void MessageRemindForm::focusOutEvent(QFocusEvent *event)
{//如果失去焦点,就把提示框隐藏
	if(event->lostFocus())
	{
		view->hide();
	}
}

void MessageRemindForm::slotClickedItem(const QModelIndex &index)
{
	//qDebug()<<"在这里处理按下一个item1111111111111111";
	emit signalClickedItem(index);
	model->removeRow(index.row());
	//点击一条消息就要删除这一项
	--count;
	if(0 == count)
	{
		qDebug()<<"0 == count";
		emit signalMessageReminFormClose(this);
		if(!view->isHidden())
		{
			view->hide();
		}
		this->close();
	}
}

void MessageRemindForm::initSetting()
{
	this->setMouseTracking(true);//这个比较重要,激活鼠标追踪
	this->setWindowFlags(Qt::FramelessWindowHint);
	int x = qApp->desktop()->width();
	int y = 0;
	move(x, y);
}

void MessageRemindForm::initData()
{
	view = new MyListView(this);
	model = new QStandardItemModel(this);
	view->setWindowFlags(Qt::ToolTip);
	connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(slotClickedItem(QModelIndex)));
	//connect(view, SIGNAL())
	count = 0;
}
