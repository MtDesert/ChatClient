#ifndef SNAPSCOPE_H
#define SNAPSCOPE_H

#include <QWidget>

class SnapScope : public QWidget
{
	Q_OBJECT
/*
这是一个相当于放大镜的一个部件,它一直跟随着鼠标
*/
public:
	explicit SnapScope(QWidget *parent = 0);
	~SnapScope();

	QWidget* ptrImageSource;//这个可以获取图像源
	void reLocate(QPoint mousePos);//用来重定位的
protected:
	void mouseMoveEvent(QMouseEvent *mouse);
	void moveEvent(QMoveEvent *move);
	void paintEvent(QPaintEvent *paint);
private:
	QRect snapRect;//显示的区域
	QPixmap snapPixmap;
};

#endif // SNAPSCOPE_H