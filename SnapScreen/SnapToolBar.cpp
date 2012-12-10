#include <QtGui>
#include "SnapToolBar.h"

SnapToolBar::SnapToolBar(QWidget *parent) :
	QToolBar(parent)
{
	setWindowFlags(Qt::ToolTip);//这样可以去掉边框
	//初始化所有菜单
	initShapeMenu();
	initPenMenu();
	initBrushMenu();
	addAction(QString::fromUtf8("绘图"))->setMenu(&shapeMenu);
	addAction(QString::fromUtf8("画笔"))->setMenu(&penMenu);
	addAction(QString::fromUtf8("画刷"))->setMenu(&brushMenu);
	addAction(QString::fromUtf8("取消"));
	addAction(QString::fromUtf8("完成"));
	//连接信号与槽
	connect(&penMenu,SIGNAL(triggered(QAction*)),this,SLOT(slotPenMenuTriggered(QAction*)));
	connect(&brushMenu,SIGNAL(triggered(QAction*)),this,SLOT(slotBrushMenuTriggered(QAction*)));
	connect(&brushStyleMenu,SIGNAL(triggered(QAction*)),this,SLOT(slotSetBrushStyle(QAction*)));
	connect(&penCapStyleMenu,SIGNAL(triggered(QAction*)),this,SLOT(slotSetPenCapStyle(QAction*)));
	connect(&penJoinStyleMenu,SIGNAL(triggered(QAction*)),this,SLOT(slotSetPenJoinStyle(QAction*)));
	connect(&penStyleMenu,SIGNAL(triggered(QAction*)),this,SLOT(slotSetPenStyle(QAction*)));
	connect(this,SIGNAL(actionTriggered(QAction*)),this,SLOT(slotActionTriggered(QAction*)));
}
SnapToolBar::~SnapToolBar()
{
	qDebug()<<"SnapToolBar::~SnapToolBar()";
}

//接下来是一堆初始化函数
void SnapToolBar::initShapeMenu()
{
	shapeGroup=new QActionGroup(NULL);
	shapeGroup->addAction(shapeMenu.addAction(QString::fromUtf8("无")))->setCheckable(true);
	shapeGroup->addAction(shapeMenu.addAction(QString::fromUtf8("普通")))->setCheckable(true);
	shapeGroup->addAction(shapeMenu.addAction(QString::fromUtf8("直线")))->setCheckable(true);
	shapeGroup->addAction(shapeMenu.addAction(QString::fromUtf8("矩形")))->setCheckable(true);
	shapeGroup->addAction(shapeMenu.addAction(QString::fromUtf8("椭圆")))->setCheckable(true);
	shapeGroup->addAction(shapeMenu.addAction(QString::fromUtf8("填充")))->setCheckable(true);
	shapeGroup->addAction(shapeMenu.addAction(QString::fromUtf8("文字")))->setCheckable(true);
	shapeGroup->actions().at(0)->setChecked(true);
}
void SnapToolBar::initPenMenu()
{
	initPenCapStyleMenu();
	initPenJoinStyleMenu();
	initPenStyleMenu();
	penMenu.addMenu(&penCapStyleMenu)->setText(QString::fromUtf8("拐点风格"));
	penMenu.addMenu(&penJoinStyleMenu)->setText(QString::fromUtf8("连接风格"));
	penMenu.addMenu(&penStyleMenu)->setText(QString::fromUtf8("线型风格"));
	penMenu.addAction(QString::fromUtf8("装饰"))->setCheckable(true);
	penMenu.addAction(QString::fromUtf8("破折偏移"));
	penMenu.addAction(QString::fromUtf8("斜接限度"));
	penMenu.addAction(QString::fromUtf8("线宽"));
	penMenu.addAction(QString::fromUtf8("颜色"));
}
void SnapToolBar::initPenCapStyleMenu()
{
	penCapStyleGroup=new QActionGroup(NULL);
	penCapStyleGroup->addAction(penCapStyleMenu.addAction(QString::fromUtf8("平")))->setCheckable(true);
	penCapStyleGroup->addAction(penCapStyleMenu.addAction(QString::fromUtf8("方")))->setCheckable(true);
	penCapStyleGroup->addAction(penCapStyleMenu.addAction(QString::fromUtf8("圆")))->setCheckable(true);
	penCapStyleGroup->actions().at(0)->setChecked(true);
}
void SnapToolBar::initPenJoinStyleMenu()
{
	penJoinStyleGroup=new QActionGroup(NULL);
	penJoinStyleGroup->addAction(penJoinStyleMenu.addAction(QString::fromUtf8("斜接")))->setCheckable(true);
	penJoinStyleGroup->addAction(penJoinStyleMenu.addAction(QString::fromUtf8("斜角")))->setCheckable(true);
	penJoinStyleGroup->addAction(penJoinStyleMenu.addAction(QString::fromUtf8("圆接")))->setCheckable(true);
	penJoinStyleGroup->addAction(penJoinStyleMenu.addAction(QString::fromUtf8("SVG斜接")))->setCheckable(true);
	penJoinStyleGroup->actions().at(0)->setChecked(true);
}
void SnapToolBar::initPenStyleMenu()
{
	penStyleGroup=new QActionGroup(NULL);
	penStyleGroup->addAction(penStyleMenu.addAction(QString::fromUtf8("无")))->setCheckable(true);
	penStyleGroup->addAction(penStyleMenu.addAction(QString::fromUtf8("固体")))->setCheckable(true);
	penStyleGroup->addAction(penStyleMenu.addAction(QString::fromUtf8("破折号")))->setCheckable(true);
	penStyleGroup->addAction(penStyleMenu.addAction(QString::fromUtf8("点")))->setCheckable(true);
	penStyleGroup->addAction(penStyleMenu.addAction(QString::fromUtf8("破折点")))->setCheckable(true);
	penStyleGroup->addAction(penStyleMenu.addAction(QString::fromUtf8("破折点点")))->setCheckable(true);
	penStyleGroup->addAction(penStyleMenu.addAction(QString::fromUtf8("自定义破折")))->setCheckable(true);
	penStyleGroup->actions().at(0)->setChecked(true);
}
void SnapToolBar::initBrushMenu()
{
	initBrushStyleMenu();
	brushMenu.addMenu(&brushStyleMenu)->setText(QString::fromUtf8("风格"));
	brushMenu.addAction(QString::fromUtf8("颜色"));
}
void SnapToolBar::initBrushStyleMenu()
{
	brushStyleGroup=new QActionGroup(NULL);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("无")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("固体")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("密集1")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("密集2")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("密集3")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("密集4")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("密集5")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("密集6")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("密集7")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("水平")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("垂直")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("交叉")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("左斜杠")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("右斜杠")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("交叉斜杠")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("线性梯度")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("辐射梯度")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("锥形梯度")))->setCheckable(true);
	brushStyleGroup->addAction(brushStyleMenu.addAction(QString::fromUtf8("纹理")))->setCheckable(true);
	brushStyleGroup->actions().at(0)->setChecked(true);
}
//这里是一堆返回函数
QAction* SnapToolBar::getShapeAction()
{
	return shapeGroup->checkedAction();
}
QList<QAction*> SnapToolBar::getShapeGroupActions()
{
	return shapeGroup->actions();
}
QPen SnapToolBar::getPen()
{
	return pen;
}
QBrush SnapToolBar::getBrush()
{
	return brush;
}
int SnapToolBar::getShapeActionID()
{
	return shapeGroup->actions().indexOf(shapeGroup->checkedAction());
}
//接下来是一堆吐槽
void SnapToolBar::slotSetBrushStyle(QAction *action)
{
	int id=brushStyleGroup->actions().indexOf(action);
	switch(id)
	{
		case 0:
			brush.setStyle(Qt::NoBrush);
			break;
		case 1:
			brush.setStyle(Qt::SolidPattern);
			break;
		case 2:
			brush.setStyle(Qt::Dense1Pattern);
			break;
		case 3:
			brush.setStyle(Qt::Dense2Pattern);
			break;
		case 4:
			brush.setStyle(Qt::Dense3Pattern);
			break;
		case 5:
			brush.setStyle(Qt::Dense4Pattern);
			break;
		case 6:
			brush.setStyle(Qt::Dense5Pattern);
			break;
		case 7:
			brush.setStyle(Qt::Dense6Pattern);
			break;
		case 8:
			brush.setStyle(Qt::Dense7Pattern);
			break;
		case 9:
			brush.setStyle(Qt::HorPattern);
			break;
		case 10:
			brush.setStyle(Qt::VerPattern);
			break;
		case 11:
			brush.setStyle(Qt::CrossPattern);
			break;
		case 12:
			brush.setStyle(Qt::BDiagPattern);
			break;
		case 13:
			brush.setStyle(Qt::FDiagPattern);
			break;
		case 14:
			brush.setStyle(Qt::DiagCrossPattern);
			break;
		case 15:
			brush.setStyle(Qt::LinearGradientPattern);
			break;
		case 16:
			brush.setStyle(Qt::RadialGradientPattern);
			break;
		case 17:
			brush.setStyle(Qt::ConicalGradientPattern);
			break;
		case 18:
			brush.setStyle(Qt::TexturePattern);
			break;
		default:;
	}
}
void SnapToolBar::slotSetPenCapStyle(QAction *action)
{
	int id=penCapStyleGroup->actions().indexOf(action);
	switch(id)
	{
		case 0:
			pen.setCapStyle(Qt::FlatCap);
			break;
		case 1:
			pen.setCapStyle(Qt::SquareCap);
			break;
		case 2:
			pen.setCapStyle(Qt::RoundCap);
			break;
		default:;
	}
}
void SnapToolBar::slotSetPenJoinStyle(QAction *action)
{
	int id=penJoinStyleGroup->actions().indexOf(action);
	switch(id)
	{
		case 0:
			pen.setJoinStyle(Qt::MiterJoin);
			break;
		case 1:
			pen.setJoinStyle(Qt::BevelJoin);
			break;
		case 2:
			pen.setJoinStyle(Qt::RoundJoin);
			break;
		case 3:
			pen.setJoinStyle(Qt::SvgMiterJoin);
			break;
		default:;
	}
}
void SnapToolBar::slotSetPenStyle(QAction *action)
{
	int id=penStyleGroup->actions().indexOf(action);
	switch(id)
	{
		case 0:
			pen.setStyle(Qt::NoPen);
			break;
		case 1:
			pen.setStyle(Qt::SolidLine);
			break;
		case 2:
			pen.setStyle(Qt::DashDotLine);
			break;
		case 3:
			pen.setStyle(Qt::DotLine);
			break;
		case 4:
			pen.setStyle(Qt::DashDotLine);
			break;
		case 5:
			pen.setStyle(Qt::DashDotDotLine);
			break;
		case 6:
			pen.setStyle(Qt::CustomDashLine);
			break;
		default:;
	}
}
void SnapToolBar::slotPenMenuTriggered(QAction *action)
{
	int id=penMenu.actions().indexOf(action);
	switch(id)
	{
		case 3:
			if(action->isChecked())
			{
				pen.setCosmetic(true);
			}
			else
			{
				pen.setCosmetic(false);
			}
			break;
		case 4:
		{
			bool OK;
			double newValue=QInputDialog::getDouble
			(
			this,
			QString::fromUtf8("破折偏移"),
			QString::fromUtf8("偏移值"),
			pen.miterLimit(),
			0,255,1,&OK
			);
			if(OK)
			{
				pen.setMiterLimit(newValue);
			}
		}
			break;
		case 5:
		{
			bool OK;
			double newValue=QInputDialog::getDouble
			(
			this,
			QString::fromUtf8("斜接限度"),
			QString::fromUtf8("限度值"),
			pen.miterLimit(),
			0,255,1,&OK
			);
			if(OK)
			{
				pen.setMiterLimit(newValue);
			}
		}
			break;
		case 6:
		{
			bool OK;
			double newValue=QInputDialog::getDouble
			(
			this,
			QString::fromUtf8("画笔宽度"),
			QString::fromUtf8("宽度值"),
			pen.width(),
			0,255,1,&OK
			);
			if(OK)
			{
				pen.setWidthF(newValue);
			}
		}
			break;
		case 7:
		{
			QColor newColor=QColorDialog::getColor
			(
			pen.color(),
			this,
			QString::fromUtf8("画笔颜色")
			);
			if(newColor.isValid())
			{
				pen.setColor(newColor);
			}
		}
			break;
		default:;
	}
}
void SnapToolBar::slotBrushMenuTriggered(QAction *action)
{
	int id=brushMenu.actions().indexOf(action);
	switch(id)
	{
		case 1:
		{
			QColor newColor=QColorDialog::getColor(brush.color(),this,QString::fromUtf8("画刷颜色"));
			if(newColor.isValid())
			{
				brush.setColor(newColor);
			}
		}
			break;
		default:;
	}
}
void SnapToolBar::slotActionTriggered(QAction *action)
{
	if(action->text()==QString::fromUtf8("完成"))
	{
		emit signalFinish();
	}
	if(action->text()==QString::fromUtf8("取消"))
	{
		emit signalCancel();
	}
}
//这些是事件函数
void SnapToolBar::mousePressEvent(QMouseEvent *mouse)
{
	clickPos=mouse->pos();
}
void SnapToolBar::mouseMoveEvent(QMouseEvent *mouse)
{
	move(mouse->globalPos()-clickPos);
}