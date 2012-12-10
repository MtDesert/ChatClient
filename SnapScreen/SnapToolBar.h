#ifndef SNAPTOOLBAR_H
#define SNAPTOOLBAR_H

#include <QtGui>
#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
class SnapToolBar : public QToolBar
{
	Q_OBJECT
/*
这个类是绘图工具条,能进行画笔画刷的设置,其它类可以从它这里取到画笔和画刷的设置值
当然,这里面也可以选择绘图图形,也可以获取相应的值
*/
public:
	explicit SnapToolBar(QWidget *parent = 0);
	~SnapToolBar();

	QAction* getShapeAction();
	QList<QAction*> getShapeGroupActions();
	int getShapeActionID();
	QPen getPen();
	QBrush getBrush();
protected:
	void mousePressEvent(QMouseEvent *mouse);
	void mouseMoveEvent(QMouseEvent *mouse);
signals:
	void signalFinish();//当截图完成时发出此信号
	void signalCancel();//当取消截图时发出此信号
private slots:
	void slotActionTriggered(QAction *action);
	void slotSetPenCapStyle(QAction *action);
	void slotSetPenJoinStyle(QAction *action);
	void slotSetPenStyle(QAction *action);
	void slotSetBrushStyle(QAction *action);
	void slotPenMenuTriggered(QAction *action);
	void slotBrushMenuTriggered(QAction *action);
private:
	//这是主要成员
	QPalette toolBarPalette;//可恶,居然还要设置调色板
	QPen pen;//…这是笔
	QBrush brush;//这是刷…
	QPoint clickPos;//移动时用到
	//这是图形的菜单
	QMenu shapeMenu;
	QActionGroup *shapeGroup;
	void initShapeMenu();
	//这是画笔的菜单
	QMenu penMenu;
	QMenu penCapStyleMenu;//隶属于penMenu
	QMenu penJoinStyleMenu;//隶属于penMenu
	QMenu penStyleMenu;//隶属于penMenu
	QActionGroup *penCapStyleGroup;
	QActionGroup *penJoinStyleGroup;
	QActionGroup *penStyleGroup;
	void initPenMenu();
	void initPenCapStyleMenu();
	void initPenJoinStyleMenu();
	void initPenStyleMenu();
	//这是画刷的菜单
	QMenu brushMenu;
	QMenu brushStyleMenu;
	QActionGroup *brushStyleGroup;
	void initBrushMenu();
	void initBrushStyleMenu();
};

#endif // SNAPTOOLBAR_H
