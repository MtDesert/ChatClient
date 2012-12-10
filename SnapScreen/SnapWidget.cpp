#include <QApplication>
#include <QStack>
#include "SnapWidget.h"

SnapWidget::SnapWidget(QWidget *parent) :
QWidget(parent)
{
	//初始化当前状态
	setWindowFlags(Qt::ToolTip);
	resize(0,0);
	setMouseTracking(true);
	//初始化编辑状态
	editing=false;
	visibleTextBorder=false;
	visibleText=false;
	//连接信号与槽
	connect(&toolBar,SIGNAL(signalCancel()),this,SIGNAL(signalClose()));//接收取消信号
	connect(&toolBar,SIGNAL(signalFinish()),this,SLOT(slotSnapFinish()));//接收截图信号
}
SnapWidget::~SnapWidget()
{
	qDebug()<<"SnapWidget::~SnapWidget()";
}

//这些是事件函数
void SnapWidget::mouseMoveEvent(QMouseEvent *mouse)
{
	globalCurrentPos=mouse->globalPos();
	widgetCurrentPos=mouse->pos();
	if(mouse->buttons()==Qt::NoButton)
	{
		/*rgbValue=snapImage.pixel(mouse->pos());
		QToolTip::showText
		(
		mouse->globalPos(),
		QString::fromUtf8("屏幕坐标:")+QString::number(mouse->globalX())+","+QString::number(mouse->globalY())+"\n"+
		QString::fromUtf8("图内坐标:")+QString::number(mouse->x())+","+QString::number(mouse->y())+"\n"+
		QString::fromUtf8("颜色:")+QString::number(qRed(rgbValue))+","+QString::number(qGreen(rgbValue))+","+QString::number(qBlue(rgbValue)),
		this
		);*/
	}
	if(mouse->buttons()==Qt::LeftButton)//只按左键移动
	{
		int id=toolBar.getShapeActionID();
		switch(id)
		{
			case 0://移动动作
				move(globalCurrentPos-widgetPressedPos);
				/*QToolTip::showText
				(
				mouse->globalPos(),
				QString::fromUtf8("左上↖坐标:")+QString::number(snapGeometry.topLeft().x())+","+QString::number(snapGeometry.topLeft().y())+"\n"+
				QString::fromUtf8("左下↙坐标:")+QString::number(snapGeometry.bottomLeft().x())+","+QString::number(snapGeometry.bottomLeft().y())+"\n"+
				QString::fromUtf8("右上↗坐标:")+QString::number(snapGeometry.topRight().x())+","+QString::number(snapGeometry.topRight().y())+"\n"+
				QString::fromUtf8("右下↘坐标:")+QString::number(snapGeometry.bottomRight().x())+","+QString::number(snapGeometry.bottomRight().y()),
				this
				);*/
				break;
			case 1://任意画
				painterPath.lineTo(widgetCurrentPos);
				update();
				break;
			case 2:case 3:case 4:case 5:
				update();
				break;
			case 6://插文字
				//绘制文本框
				if(widgetCurrentPos.x()>widgetPressedPos.x())
				{
					if(widgetCurrentPos.y()<widgetPressedPos.y())//第1象限
					{
						textRect.setBottomLeft(widgetPressedPos);
						textRect.setTopRight(widgetCurrentPos);
					}
					if(widgetCurrentPos.y()>widgetPressedPos.y())//第4象限
					{
						textRect.setTopLeft(widgetPressedPos);
						textRect.setBottomRight(widgetCurrentPos);
					}
				}
				if(widgetCurrentPos.x()<widgetPressedPos.x())
				{
					if(widgetCurrentPos.y()<widgetPressedPos.y())//第2象限
					{
						textRect.setBottomRight(widgetPressedPos);
						textRect.setTopLeft(widgetCurrentPos);
					}
					if(widgetCurrentPos.y()>widgetPressedPos.y())//第3象限
					{
						textRect.setTopRight(widgetPressedPos);
						textRect.setBottomLeft(widgetCurrentPos);
					}
				}
				update();
				break;
			default:;
		}
	}
	ptrSnapScope->reLocate(globalCurrentPos);
}
void SnapWidget::mousePressEvent(QMouseEvent *mouse)
{
	globalPressedPos=mouse->globalPos();
	widgetPressedPos=mouse->pos();
	if(mouse->buttons()==Qt::LeftButton)//只按下左键的情况
	{
		int id=toolBar.getShapeActionID();
		if(id==0)
		{
			setCursor(Qt::SizeAllCursor);
		}
		else
		{
			setCursor(Qt::CrossCursor);
			editing=true;
			switch(id)
			{
				case 1:
					painterPath.moveTo(widgetPressedPos);
					update();
					break;
				case 5:
					fillImage(snapImage,mouse->pos(),toolBar.getBrush().color().rgb());
					update();
					break;
				case 6:
					visibleTextBorder=true;
					break;
				default:;
			}
		}
	}
}
void SnapWidget::mouseReleaseEvent(QMouseEvent *mouse)
{
	if(mouse->button()==Qt::LeftButton&&mouse->buttons()==Qt::NoButton)//最后放开左键的情况
	{
		snapImage=QPixmap::grabWidget(this,rect()).toImage();
		//非0的ID是绘图动作,保存
		int id=toolBar.getShapeActionID();
		if(id!=0&&id!=6)//插文字的情况比较特殊
		{
			pixmapStack.push(QPixmap::fromImage(snapImage));
		}
		toolBar.raise();
		switch(id)
		{
			case 1://清空路径
				painterPath=QPainterPath();
				break;
			case 6://文本框绘制结束,设置字体
				if(textRect.isValid())
				{
					bool ok;
					textFont=QFontDialog::getFont(&ok,textEdit.font(),this,QString::fromUtf8("设置字体"));
					if(ok)
					{
						textEdit.setFont(textFont);
						textEdit.activateWindow();
						visibleText=true;
						connect(&textEdit,SIGNAL(textChanged()),this,SLOT(update()));
						connect(&textEdit,SIGNAL(signalFinishTyped()),this,SLOT(slotFinishTyped()));
					}
					else
					{
						textRect.setSize(QSize(0,0));
						textEdit.setGeometry(textRect);
						visibleTextBorder=false;
						visibleText=false;
					}
				}
				break;
			default:;
		}
		//其它改变
		setCursor(Qt::ArrowCursor);
		if(!textRect.isValid())//这是为了排除文本编辑的情况
		{
			editing=false;
		}
		update();
	}
	if(mouse->button()==Qt::RightButton&&mouse->buttons()==Qt::NoButton)//最后放开右键的情况
	{
		if(pixmapStack.isEmpty())
		{
			resize(0,0);
			toolBar.setVisible(false);
			ptrSnapScope->ptrImageSource=ptrScreenWidget;
		}
		else
		{
			pixmapStack.pop();
			if(pixmapStack.isEmpty())
			{
				snapImage=snapPixmap.toImage();
			}
			else
			{
				snapImage=pixmapStack.top().toImage();
			}
			update();
			toolBar.activateWindow();
		}
	}
}
void SnapWidget::paintEvent(QPaintEvent *paint)
{
	QPainter painter(this);
	//先画背景图
	if(pixmapStack.isEmpty())
	{
		painter.drawPixmap(rect(),snapPixmap);
	}
	else
	{
		painter.drawPixmap(rect(),pixmapStack.top());
	}
	//再话我们所绘制的图形
	if(editing)
	{
		int id=toolBar.getShapeActionID();
		if(id==6)//这是用来绘制"文本框"的
		{
			painter.setPen(Qt::SolidLine);
			painter.setBrush(Qt::NoBrush);
		}
		else
		{
			painter.setPen(toolBar.getPen());
			painter.setBrush(toolBar.getBrush());
		}
		switch(id)
		{
			case 1://任意画
				painter.drawPoint(widgetPressedPos);
				painter.drawPath(painterPath);
				break;
			case 2://直线
				painter.drawLine(widgetPressedPos,widgetCurrentPos);
				break;
			case 3://矩形
				painter.drawRect
				(
				widgetPressedPos.x(),
				widgetPressedPos.y(),
				widgetCurrentPos.x()-widgetPressedPos.x(),
				widgetCurrentPos.y()-widgetPressedPos.y()
				);
				break;
			case 4://椭圆
				painter.drawEllipse
				(
				widgetPressedPos.x(),
				widgetPressedPos.y(),
				widgetCurrentPos.x()-widgetPressedPos.x(),
				widgetCurrentPos.y()-widgetPressedPos.y()
				);
				break;
			case 5://填充
				painter.drawImage(QPoint(0,0),snapImage);
				break;
			case 6://文字
				if(textRect.isValid()&&visibleTextBorder)//绘制矩形框
				{
					painter.drawRect(textRect);
				}
				if(visibleText)//再绘制文字
				{
					painter.setPen(toolBar.getPen());
					painter.setFont(textFont);
					painter.drawText(textRect,Qt::TextWrapAnywhere,textEdit.toPlainText());
				}
				break;
			default:;
		}
	}
}
void SnapWidget::moveEvent(QMoveEvent *move)
{
	//一旦移动就更新内容
	snapGeometry=geometry();
	snapPixmap=QPixmap::grabWidget(ptrScreenWidget,snapGeometry);
	update();
}
void SnapWidget::resizeEvent(QResizeEvent *resize)
{
	//调了大小也要更新内容
	snapGeometry=geometry();
	if(snapGeometry.isValid())
	{
		snapPixmap=QPixmap::grabWidget(ptrScreenWidget,snapGeometry);
		update();
	}
	else
	{
		while(!pixmapStack.isEmpty())//清空原来的数据
		{
			pixmapStack.pop();
		}
		toolBar.setVisible(false);
		textEdit.resize(0,0);
	}
}
void SnapWidget::enterEvent(QEvent *event)
{
	if(toolBar.getShapeActionID()==0)
	{
		setCursor(QCursor(Qt::ArrowCursor));
	}
	else
	{
		setCursor(QCursor(Qt::CrossCursor));
	}
}
void SnapWidget::leaveEvent(QEvent *event)
{
	setCursor(QCursor(Qt::ArrowCursor));
}
void SnapWidget::closeEvent(QCloseEvent *event)
{
	toolBar.close();
	textEdit.close();
}

//这是槽函数
void SnapWidget::slotSnapFinish()//把最终看到的内容作为截图内容放入剪贴板
{
	QClipboard *clipboard=QApplication::clipboard();
	if(pixmapStack.isEmpty())
	{
		clipboard->setPixmap(snapPixmap);
	}
	else
	{
		clipboard->setPixmap(pixmapStack.top());
	}
	emit signalClose();
}
void SnapWidget::slotGetSnap()//接收到截图信号后应该做的事
{
	//转换成QImage类型可以取像素的颜色值
	snapImage=snapPixmap.toImage();
	//显示工具条
	toolBar.move(pos());
	toolBar.setVisible(true);
	toolBar.activateWindow();
}
void SnapWidget::slotFinishTyped()
{
	//去掉边框,将结果保存
	visibleTextBorder=false;
	repaint();//似乎repaint()是同步的,update()是异步的
	snapImage=QPixmap::grabWidget(this,rect()).toImage();
	pixmapStack.push(QPixmap::fromImage(snapImage));
	visibleText=false;
	//处理textEdit部件
	disconnect(&textEdit,SIGNAL(textChanged()),this,SLOT(update()));
	disconnect(&textEdit,SIGNAL(signalFinishTyped()),this,SLOT(slotFinishTyped()));
	textRect.setSize(QSize(0,0));
	textEdit.setPlainText(QString());
	qDebug()<<"slotFinishTyped() over";
}
//这是大漠的截图最高成就:种子边界填充算法
void SnapWidget::fillImage(QImage &image,QPoint pos,QRgb fillColor)
{
	QRgb oldColor=image.pixel(pos);//获取被填充区域的原始颜色
	if(fillColor==oldColor)//若颜色一样则不填充
	{
		qDebug()<<"color same";
		return;
	}
	//这是装边界点的容器
	QVector<QPoint> vectorEdgePoint(image.width()*image.height(),QPoint(-1,-1));//这是用来保存边界点的
	//QPoint* ptrVectorEdgePoint=vectorEdgePoint.data();
	int vectorWrite=0;//这是写入的地址下标
	int vectorRead=0;//这是读取的地址下标
	int vectorCurrent=0;//这是用来扫描有没有重复的点的
	//这堆是推理用的点
	QPoint upPoint;//上点
	QPoint downPoint;//下点
	QPoint leftPoint;//左点
	QPoint rightPoint;//右点
	QPoint centerPoint;//中点
	//先把起点扔入容器
	vectorEdgePoint[vectorWrite]=pos;
	++vectorWrite;
	//开始填充
	while(vectorWrite!=vectorRead)//填充完所有应该填充的点
	{
		//取出当前点并且填充
		centerPoint=vectorEdgePoint[vectorRead];
		image.setPixel(centerPoint,fillColor);
		//下面的是向四周扩散的
		if(centerPoint.x()>0)//向左填充(如果可行的话)
		{
			leftPoint=centerPoint+QPoint(-1,0);
			if(image.pixel(leftPoint)==oldColor)//与原来的颜色一样才能填充
			{
				//扫描此点是否已经出现在队列中
				for
				(
				vectorCurrent=vectorWrite-1;
				vectorCurrent>vectorRead&&vectorEdgePoint[vectorCurrent]!=leftPoint;
				--vectorCurrent
				);
				//如果不出现在历史队列中则添加
				if(vectorCurrent==vectorRead)
				{
					vectorEdgePoint[vectorWrite]=leftPoint;
					++vectorWrite;
				}
			}
		}
		if(centerPoint.x()<image.width()-1)//向右填充(如果可行的话)
		{
			rightPoint=centerPoint+QPoint(1,0);
			if(image.pixel(rightPoint)==oldColor)
			{
				//扫描此点是否已经出现在队列中
				for
				(
				vectorCurrent=vectorWrite-1;
				vectorCurrent>vectorRead&&vectorEdgePoint[vectorCurrent]!=rightPoint;
				--vectorCurrent
				);
				//如果不出现在历史队列中则添加
				if(vectorCurrent==vectorRead)
				{
					vectorEdgePoint[vectorWrite]=rightPoint;
					++vectorWrite;
				}
			}
		}
		if(centerPoint.y()>0)//向上填充(如果可行的话)
		{
			upPoint=centerPoint+QPoint(0,-1);
			if(image.pixel(upPoint)==oldColor)
			{
				//扫描此点是否已经出现在队列中
				for
				(
				vectorCurrent=vectorWrite-1;
				vectorCurrent>vectorRead&&vectorEdgePoint[vectorCurrent]!=upPoint;
				--vectorCurrent
				);
				//如果不出现在历史队列中则添加
				if(vectorCurrent==vectorRead)
				{
					vectorEdgePoint[vectorWrite]=upPoint;
					++vectorWrite;
				}
			}
		}
		if(centerPoint.y()<image.height()-1)//向下填充
		{
			downPoint=centerPoint+QPoint(0,1);
			if(image.pixel(downPoint)==oldColor)
			{
				//扫描此点是否已经出现在队列中
				for
				(
				vectorCurrent=vectorWrite-1;
				vectorCurrent>vectorRead&&vectorEdgePoint[vectorCurrent]!=downPoint;
				--vectorCurrent
				);
				//如果不出现在历史队列中则添加
				if(vectorCurrent==vectorRead)
				{
					vectorEdgePoint[vectorWrite]=downPoint;
					++vectorWrite;
				}
			}
		}
		//当前点已经处理了,跳过
		++vectorRead;
	}
	qDebug()<<"fill end";
}