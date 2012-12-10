#include "SnapScreen.h"
#include <QApplication>
#include <QPainter>
#include <QColor>
#include <QDesktopWidget>
#include <QClipboard>
#include <QToolTip>
#include <QDebug>

SnapScreen::SnapScreen(QWidget *parent) :
QWidget(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
	//显示按下截图时瞬间的图像(即原图)
	screenImage=QPixmap::grabWindow
	(
	QApplication::desktop()->winId(),0,0,
	QApplication::desktop()->width(),
	QApplication::desktop()->height()
	).toImage();
	screenWidget.setWindowState(Qt::WindowFullScreen);
	screenPalette.setBrush(backgroundRole(),QBrush(screenImage));
	screenWidget.setPalette(screenPalette);
	screenWidget.setVisible(true);
	//进入截图模式
	snapPalette.setColor(backgroundRole(),QColor(0,0,0));
	setPalette(snapPalette);//设置有色滤镜
	setWindowOpacity(0.5);//设置透明度以看见后面的画面
	setWindowState(Qt::WindowFullScreen);//截图的可选择区域是整个屏幕
	setMouseTracking(true);//这是为了让鼠标没按下时也能触发mouseMoveEvent,以便显示当前点的坐标值和颜色值
	//这个是截图器
	snapWidget.ptrScreenWidget=&screenWidget;
	snapWidget.ptrSnapScope=&snapScope;
	snapWidget.setVisible(true);
	//这个是一个小型放大镜,用于精确观察
	snapScope.ptrImageSource=&screenWidget;
	snapScope.setVisible(true);
	currentPos=QCursor::pos();
	//连接信号和槽
	connect(&snapWidget,SIGNAL(signalClose()),this,SLOT(close()));//接收关闭信号
	connect(this,SIGNAL(signalGetSnap()),&snapWidget,SLOT(slotGetSnap()));
}

SnapScreen::~SnapScreen()
{
	qDebug()<<"SnapScreen::~SnapScreen()";
}

void SnapScreen::mouseMoveEvent(QMouseEvent *mouse)
{
	currentPos=mouse->pos();
	if(mouse->buttons()==Qt::NoButton)//正常鼠标移动
	{
		//显示坐标和颜色值
		/*rgbValue=screenImage.pixel(mouse->globalPos());
		QToolTip::showText(
		mouse->globalPos(),
		QString::fromUtf8("坐标:")+QString::number(mouse->globalX())+","+QString::number(mouse->globalY())+"\n"+
		QString::fromUtf8("颜色:")+QString::number(qRed(rgbValue))+","+QString::number(qGreen(rgbValue))+","+QString::number(qBlue(rgbValue)),
		this
		);*/
	}
	if(mouse->buttons()==Qt::LeftButton)//只按下左键移动
	{
		//绘制截图框
		if(mouse->x()>pressedPos.x())
		{
			if(mouse->y()<pressedPos.y())//第1象限
			{
				snapArea.setBottomLeft(pressedPos);
				snapArea.setTopRight(mouse->pos());
			}
			if(mouse->y()>pressedPos.y())//第4象限
			{
				snapArea.setTopLeft(pressedPos);
				snapArea.setBottomRight(mouse->pos());
			}
		}
		if(mouse->x()<pressedPos.x())
		{
			if(mouse->y()<pressedPos.y())//第2象限
			{
				snapArea.setBottomRight(pressedPos);
				snapArea.setTopLeft(mouse->pos());
			}
			if(mouse->y()>pressedPos.y())//第3象限
			{
				snapArea.setTopRight(pressedPos);
				snapArea.setBottomLeft(mouse->pos());
			}
		}
		snapWidget.setGeometry(snapArea);
		/*QToolTip::showText
		(
		mouse->globalPos(),
		QString::fromUtf8("起点坐标:")+QString::number(pressedPos.x())+","+QString::number(pressedPos.y())+"\n"+
		QString::fromUtf8("当前坐标:")+QString::number(mouse->globalX())+","+QString::number(mouse->globalY())+"\n"+
		QString::fromUtf8("区域位置:")+QString::number(snapArea.x())+","+QString::number(snapArea.y())+"\n"+
		QString::fromUtf8("区域尺寸:")+QString::number(snapArea.width())+","+QString::number(snapArea.height()),
		this
		);*/
	}
	snapScope.reLocate(currentPos);
}
void SnapScreen::mousePressEvent(QMouseEvent *mouse)
{
	if(mouse->buttons()==Qt::LeftButton)//只按下左键
	{
		pressedPos=mouse->pos();
	}
}
void SnapScreen::mouseReleaseEvent(QMouseEvent *mouse)
{
	if(mouse->button()==Qt::LeftButton&&mouse->buttons()==Qt::NoButton)//最后放开的是左键
	{
		if(snapWidget.geometry().isValid())
		{
			snapScope.ptrImageSource=&snapWidget;
			emit signalGetSnap();
		}
	}
	if(mouse->button()==Qt::RightButton&&mouse->buttons()==Qt::NoButton)//最后放开的是右键
	{
		if(snapWidget.geometry().isValid())
		{
			snapWidget.resize(0,0);
			snapScope.ptrImageSource=&screenWidget;
		}
		else
		{
			close();
		}
	}
}
void SnapScreen::closeEvent(QCloseEvent *event)
{
	screenWidget.close();
	snapWidget.close();
	snapScope.close();
}