#ifndef SNAPWIDGET_H
#define SNAPWIDGET_H

#include <QtGui>
#include "SnapToolBar.h"
#include "SnapScope.h"
#include "SnapText.h"

class SnapWidget : public QWidget
{
	Q_OBJECT
/*
这个是个截图器的类,你所看到的都是它所截取的内容
这个类可以移动,当然也可以在其上面作图,不过作图的话要跟作图工具条配套使用
*/
public:
	explicit SnapWidget(QWidget *parent = 0);
	~SnapWidget();

	QWidget* ptrScreenWidget;//这是指向图像控件的指针,靠它来获取数据
	SnapScope* ptrSnapScope;//这个是用来控制放大镜的
protected:
	void mousePressEvent(QMouseEvent *mouse);
	void mouseReleaseEvent(QMouseEvent *mouse);
	void mouseMoveEvent(QMouseEvent *mouse);
	void moveEvent(QMoveEvent *move);
	void resizeEvent(QResizeEvent *resize);
	void paintEvent(QPaintEvent *paint);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	void closeEvent(QCloseEvent *event);
signals:
	//void signalCancel();
	void signalClose();
private slots:
	void slotSnapFinish();
	void slotGetSnap();
	void slotFinishTyped();
private:
	QRect snapGeometry;//这是截图区域的矩形
	QPoint globalPressedPos;//这是按下鼠标时,鼠标在屏幕中的坐标
	QPoint widgetPressedPos;//这是按下鼠标时,鼠标在部件中的坐标
	QPoint globalCurrentPos;//这是鼠标在移动时在屏幕的坐标
	QPoint widgetCurrentPos;//这是鼠标在移动时在部件的坐标
	QRgb rgbValue;//这是当前点的颜色值

	QPainterPath painterPath;//这是画任意线条用的
	QPixmap snapPixmap;//这是截取到的图像
	QImage snapImage;//这也是截取到的图像,但他可以提取像素颜色
	QStack<QPixmap> pixmapStack;//这是个保存每一步结果的堆栈,即可以实现UNDO功能
	SnapToolBar toolBar;//画图工具条
	bool editing;//指示是否进入了编辑状态
	//关于添加文字
	QFont textFont;//这个是字体
	QRect textRect;//这是该文本框的几何形状
	SnapText textEdit;//这是插入文字用的文本框
	bool visibleTextBorder;//这是用来确定是否需要绘制文本框
	bool visibleText;//这是用来确定是否需要绘制文本
	//这就是传说中的种子填充算法
	void fillImage(QImage &image,QPoint pos,QRgb fillColor);
};

#endif // SNAPWIDGET_H
