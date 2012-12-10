#ifndef SCREENSNAP_H
#define SCREENSNAP_H

#include <QtGui>
#include <QMouseEvent>
#include <QKeyEvent>
#include "SnapToolBar.h"
#include "SnapWidget.h"
#include "SnapScope.h"

class SnapScreen : public QWidget
{
	Q_OBJECT
/*
这是类是用来显示全屏图像的
它的一个成员是screenWidget,用来保存静态图像以便给我们从中截图
但它的作用则相当于一个有色滤镜,提示用户进入了截图区域,并且能让用户看到screenWidget的图像
*/
public:
	explicit SnapScreen(QWidget *parent = 0);
	~SnapScreen();
	//这里是背景相关的成员
	QWidget screenWidget;
	QImage screenImage;//这个可以取得像素,QPixmap没有
	QPalette screenPalette;

	SnapWidget snapWidget;//这是截图部件
	SnapScope snapScope;//这是放大镜
protected:
	void mouseMoveEvent(QMouseEvent *mouse);
	void mousePressEvent(QMouseEvent *mouse);
	void mouseReleaseEvent(QMouseEvent *mouse);
	void closeEvent(QCloseEvent *event);
signals:
	void signalGetSnap();
private:
	//这是截图时用到的
	QPalette snapPalette;//这是用来当做有色滤镜的
	QPoint pressedPos;//按下鼠标按钮时,当前的鼠标坐标
	QPoint currentPos;//这是当前点
	QRect originalArea;//拖动截图区前,用于保存原位置的

	QRect snapArea;//这是我们在任意时刻看到的截图区域
	QRgb rgbValue;//这是鼠标指向的像素点的颜色值
};
#endif // SCREENSNAP_H
