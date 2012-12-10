#include "ImageLabel.h"
#include <QBuffer>
#include <QMessageBox>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QPicture>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QDebug>
#include <QPainter>
ImageLabel::ImageLabel(QWidget *parent) :
	QLabel(parent)
{
	setAcceptDrops(true);
	connect(this, SIGNAL(signalReadImageDlg()), this, SLOT(slotReadImageDlg()));
}

void ImageLabel::dragEnterEvent(QDragEnterEvent *event)
{
	if(event->mimeData()->hasFormat("text/uri-list"))
	{
		event->acceptProposedAction();
	}
	else
	{
		QLabel::dragEnterEvent(event);
	}
}

void ImageLabel::dropEvent(QDropEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	if(urls.isEmpty())
	{
		return;
	}

	if(urls.count() > 1)
	{
		QMessageBox::warning(this, tr("Error!!"), tr("Only one Pictue can move to it"), QMessageBox::Ok);
		return;
	}

	QString fileName = urls.first().toLocalFile();
	if(fileName.isEmpty())
	{
		return;
	}
	int result = QMessageBox::information(0, QString::fromUtf8("确认"), QString::fromUtf8("此操作会导致头像的更换,确定更换头像么?"), QMessageBox::Ok, QMessageBox::Cancel);
	if(QMessageBox::Ok == result)
	{//选择的是确定, 现在加载新的头像
		readImage(fileName);
	}
	else if(QMessageBox::Cancel)
	{//选择的是取消
		////不作为
	}
	else
	{//出现了意料之外的选择情况
		qDebug()<<QString::fromUtf8("尼玛又有不可预料的事情发生了");
	}
}

//大漠修改了这个函数
void ImageLabel::readImage(const QString &fileName)
{
	filePath = fileName;
	QFile file(fileName);
	if(file.open(QIODevice::ReadOnly))
	{
		qDebug()<<tr("imageLabel打开成功");
		imageDataByte=file.readAll();
		file.close();
		loadImage(imageDataByte);
	}
	else
	{
		qDebug()<<tr("imageLabel读取失败");
	}
}

QString ImageLabel::GetImagePath()
{
	return filePath;
}

void ImageLabel::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		emit signalReadImageDlg();
	}
	else
	{
		QLabel::mousePressEvent(event);
	}
}

void ImageLabel::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
	QPainterPath path;
	QRectF rect = QRectF(0,0,this->width(), this->height());
	path.addRoundRect(rect,20,20);
	QPolygon polygon= path.toFillPolygon().toPolygon();//获得这个路径上的所有的点
	QRegion region(polygon);//根据这些点构造这个区域
	setMask(region);
	p.drawPixmap(0,0,this->width(), this->height(), imageDataPix);
}

void ImageLabel::slotReadImageDlg()
{
	QString imagePath = QFileDialog::getOpenFileName(0, QString::fromLocal8Bit(""), ".", tr("image(*.jpg *.png *.jpeg)"));
	if(imagePath!="")readImage(imagePath);
}

//大漠建立了这个函数
void ImageLabel::loadImage(const QByteArray &bytes)
{
	imageDataByte=bytes;
	imageDataPix.loadFromData(imageDataByte);
	setPixmap(imageDataPix.scaled(this->size()));
	update();	//之前没有这一句......搞得没有重绘
}

void ImageLabel::loadImage(const QPixmap &pix)
{
	imageDataPix = pix;
	QBuffer buffer(&imageDataByte);
	buffer.open(QIODevice::WriteOnly);
	pix.save(&buffer, "PNG");
	update();	//之前没有这一句......搞得没有重绘
}

//用于读取头像的数据,跟loadImage正好相反
QByteArray ImageLabel::getImageDataByte()
{
	return imageDataByte;
}

QPixmap ImageLabel::getImageDataPix()
{
	return imageDataPix;
}

