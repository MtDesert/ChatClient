#include "MyListWidget.h"
#include <QMouseEvent>
#include <QDebug>
MyListWidget::MyListWidget(QWidget *parent) :
    QListWidget(parent)
{
}
//泽国写的,这个事件只接受鼠标"左键双击"
void MyListWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        QListWidget::mouseDoubleClickEvent(event);
    }
    else if(event->buttons() == Qt::RightButton)
    {
   //     qDebug()<<"event->buttons() == Qt::RightButton"<<endl;
    }
    else if(event->buttons() == Qt::MidButton)
    {
 //       qDebug()<<"event->buttons() == Qt::MidButton"<<endl;
    }
}
//大漠写的,这个事件只接受鼠标"右键单击"
void MyListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        QListWidget::mouseReleaseEvent(event);
    }
}
