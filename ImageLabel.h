#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QtGui/QLabel>
class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
class ImageLabel : public QLabel
{
	Q_OBJECT
public:
	explicit ImageLabel(QWidget *parent = 0);
	QString GetImagePath();//用来返回路径的
	void loadImage(const QByteArray &bytes);//通过QByteArray加载
	void loadImage(const QPixmap &pix);	//通过QPixmap加载

	QByteArray getImageDataByte();//大漠加的,用于读取头像数据
	QPixmap getImageDataPix();
protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void readImage(const QString &fileName);//通过文件加载

	QByteArray imageDataByte;//图像数据保存区
	QPixmap imageDataPix;
	QString filePath;//文件路径信息
public slots:
		void slotReadImageDlg();

signals:
		void signalReadImageDlg();
};
#endif
