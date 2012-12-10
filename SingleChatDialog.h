#ifndef SINGLECHATDIALOG_H
#define SINGLECHATDIALOG_H

#include "ui_SingleChatDialog.h"
#include "SnapScreen/SnapScreen.h"
#include <QFile>
#include <QDir>
#include <QSqlDatabase>
#include <ChatRecordDialog.h>
#include <QSqlQuery>
#include <QDateTime>
class SingleChatDialog : public QDialog, private Ui::SingleChatDialog
{
	Q_OBJECT

public:
	explicit SingleChatDialog(QSqlDatabase chatDb,int srcId, int desId, QString name_friend, QString name_self,QPixmap img, QWidget *parent = 0);
	void inserChatMessage(const QString &html);
	void insertNickName_DateTime(const QString &nickName, const QDateTime &time, int type);
	void writeChatLogDb(QSqlDatabase chatDb,qint32 id,QDateTime time,QString text,QString html,int type);//参数type用于写入数据库判断是用户发送还是接受，0为接受，1为发送
signals:
	void signalSendChatMessage(QByteArray &chatDataByte);
	void signalTransferFile(QByteArray &chatDataByte);
	void signalChatClose(const SingleChatDialog *p_chat);
private slots:
	void on_pushButtonClose_clicked();

	void on_pushButtonSendMessage_clicked();

	void on_pushButtonFont_clicked();

	void on_pushButtonColor_clicked();

	void on_pushButtonPicture_clicked();

	void on_pushButtonIntercept_clicked();

	void slotAddImageData(QByteArray &data);

	void on_pushButtonRecord_clicked();
	void on_textEditRecord_textChanged();
	void on_pushButtonFile_clicked();
	void on_pushButtonAbsorb_clicked();
	void slotShowColorValue();

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
private:
	void initSetting();	//一些设置相关的东西
	void initData();		//一些数据的初始化
	void initGui();
	void initChatMessage(QByteArray &byte);		//把要发送的聊天内容数字化
	void pictureToByte(QPixmap &pix, QByteArray &byte);	//把QPixmap转换到QByteArray
	void getFileIconData();
//	void pictureToByte(QImage img);	//预留
private:
	QPoint offset;					//移动的偏移量 用于重载鼠标事件实现拖动窗口
	QString strBuff;				//用于存储富文本信息
	QFont font;						//用于存储输入文本的字体
	QColor color;					//用于存储输入文本的颜色
	QString tempPath;			//记录temp的位置
	QDir *dir;							//打开temp目录
	QString imagePath;			//记录temp目录下要加在的图片的路径
	int count;							//图片张数的计数器
	QByteArray imageDataByte;	//用于存放所有要发送的图片的数据
	QDataStream *out;
	qint32 id_Friend;
	qint32 id_Self;
	QPixmap headImage;
	QString nickName_Friend;
	QString nickName_Self;
	QSqlDatabase db;
	ChatRecordDialog *chatRecordDialog;
	////////////////////////////////////////////////
	//传输文件
	QString filePath;
	QString fileName;
	QFile *file;
	qint64 totalSize;
	QByteArray fileIconData;
	bool absorb;
};

#endif // SINGLECHATDIALOG_H
