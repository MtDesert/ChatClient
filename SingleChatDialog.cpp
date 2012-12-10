#include "SingleChatDialog.h"
#include "SnapScreen/SnapScreen.h"
#include <QBuffer>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <QFontDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QTextCursor>
#include <QDateTime>
#include "RegisterDialog.h"
SingleChatDialog::SingleChatDialog(QSqlDatabase chatDb,int srcId, int desId, QString name_friend, QString name_self, QPixmap img, QWidget *parent) :
	db(chatDb),id_Self(srcId), id_Friend(desId), nickName_Friend(name_friend), nickName_Self(name_self), headImage(img), QDialog(parent)
{
	setupUi(this);

	qDebug()<<"聊天对话框初始化的时候"<<"id_Self"<<id_Self<<"id_Friend"<<id_Friend;
	initSetting();
	initData();
	initGui();
}

void SingleChatDialog::inserChatMessage(const QString &html)
{
	textEditRecord->insertChatMessage(html);
}

void SingleChatDialog::insertNickName_DateTime(const QString &nickName, const QDateTime &time, int type)
{
	qDebug()<<nickName;
	qDebug()<<time;
	qDebug()<<type;
    textEditRecord->insertNickName_DateTime(nickName, time, type);//这个函数在MyTextEdit类里面的。
}

void SingleChatDialog::initSetting()
{
	this->setWindowFlags(Qt::FramelessWindowHint);		//无边框
	this->setAttribute(Qt::WA_TranslucentBackground, true);//背景透明
	textEditChat->setFocus();
	//////////////////////////////////////////////////////////
	int x = (qApp->desktop()->width() - this->width()) / 2;
	int y = (qApp->desktop()->height() - this->height()) / 2;
	this->move(x, y);
	///////////////////////////////////////////////////////////////
	//pushButtonSendMessage->setShortcutEnabled(); 狗日的不需要设置这个东西
	pushButtonSendMessage->setShortcut(QString("Alt+s"));	//日隐式调用没有禁掉阿
	//pushButtonSendMessage->setShortcut(QString("Ctrl+Enter")); //这个竟然不可以的有点出乎意料  或者是被系统注册的快捷键不可以
	//		pushButtonSendMessage->setShortcut(QString("Ctrl+m"));	//这个是可以用的说明Ctrl的也能用,只不过刚才的特殊
	textEditRecord->setReadOnly(true);
}

void SingleChatDialog::initData()
{
	chatRecordDialog = new ChatRecordDialog(db,id_Friend,id_Self,nickName_Friend,nickName_Self);
	out = new QDataStream(&imageDataByte, QIODevice::WriteOnly);
	count = 0;
	absorb = false;
	dir = new QDir(".");
	if(!dir->exists("temp"))
	{
		dir->mkdir("temp");
	}
	dir->cd("temp");
	//tempPath = dir->absolutePath();
	connect(textEditChat, SIGNAL(signalPastePicture(QByteArray&)), this, SLOT(slotAddImageData(QByteArray&)));
}

void SingleChatDialog::initGui()
{
	labelHeadImage->loadImage(headImage);
	//labelHeadImage->setStyleSheet("border-radius:10px;");
	widgetAbsorb->setHidden(true);
}

void SingleChatDialog::initChatMessage(QByteArray &byte)
{//把要发送的聊天内容数字化
	QDataStream out(&byte, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << (qint32)0;						//占位,这里是总大小
	out << (qint32)id_Friend;			//目的id
	out << (qint32)id_Self;
	out << count << imageDataByte << textEditChat->toHtml() << textEditChat->toPlainText() <<QDateTime::currentDateTime();
	//开始计算大小1和大小2的大小了
	out.device()->seek(0);
	//大小1
	qint32 size1 = (qint32)byte.size() - (qint32)sizeof(qint32);
	out << size1;
	qDebug()<<"点击发送按钮时候"<<"id_Self"<<id_Self<<"id_Friend"<<id_Friend;
}

void SingleChatDialog::pictureToByte(QPixmap &pix, QByteArray &byte)
{//记得每次调用完了要clear一次
	QBuffer buffer(&byte);
	buffer.open(QIODevice::WriteOnly);
	pix.save(&buffer, "PNG");
	imagePath = QString("temp") + "/" + QString::number(count) + ".png";
	pix.save(imagePath, "PNG");
}

void SingleChatDialog::getFileIconData()
{
	QFileInfo fileInfor(filePath);
	QFileIconProvider fileIcon;
	QIcon icon = fileIcon.icon(fileInfor);
	if(icon.isNull())
	{
		qDebug()<<"图标是空的";
	}
	QPixmap temp = icon.pixmap(QSize(128, 128));
	fileIconData.clear();
	QBuffer buffer(&fileIconData);
	buffer.open(QIODevice::WriteOnly);
	temp.save(&buffer, "PNG");
}

void SingleChatDialog::slotShowColorValue()
{
	int x = QCursor::pos().x();
	int y = QCursor::pos().y();
	QString colorRgb;
	QString colorWeb;

	QPixmap tempPoint = QPixmap::grabWindow(QApplication::desktop()->winId(), x, y, 2, 2);

	if (!tempPoint.isNull())
	{
		QImage image = tempPoint.toImage();
		if (!image.isNull())
		{
			if (image.valid(0, 0))
			{
				QColor color = image.pixel(0, 0);
				int red=color.red();
				int green=color.green();
				int blue=color.blue();
				QString strRed=QString("%1").arg(red&0xFF,2,16,QLatin1Char('0'));
				QString strGreen=QString("%1").arg(green&0xFF,2,16,QLatin1Char('0'));
				QString strBlue=QString("%1").arg(blue&0xFF,2,16,QLatin1Char('0'));

				colorRgb = QString("%1,%2,%3").arg(red).arg(green).arg(blue);
				colorWeb = QString("#%1%2%3").arg(strRed.toUpper()).arg(strGreen.toUpper()).arg(strBlue.toUpper());
			}
		}
	}

	QString style=QString("background-color: rgb(%1);").arg(colorRgb);

	labelAbsorbColor->setStyleSheet(style);
	labelAbsorbColorRGB->setText(QString("RGB值:") + colorRgb);
	labelAbsorbColorWeb->setText(QString("Web值:") + colorWeb);
	QTimer::singleShot(100, this, SLOT(slotShowColorValue()));
}

void SingleChatDialog::slotAddImageData(QByteArray &data)
{//把一个一个的QPixmap转换成QByteArray之后就加载到imageData里面
	*out << data;
	qDebug()<<"imageDataByte"<<imageDataByte.size() <<"data"<<data.size();
}

void SingleChatDialog::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		offset = event->globalPos() - frameGeometry().topLeft();	//鼠标点击位置和窗口相对位置的0,0 的偏移量
	}
	else
	{
		QDialog::mousePressEvent(event);
	}
}

void SingleChatDialog::mouseMoveEvent(QMouseEvent *event)
{
	if(event->buttons() == Qt::LeftButton)
	{
		move(event->globalPos() - offset);

		if(!chatRecordDialog->isHidden())
		{
			int x = this->x()+ this->width();
			int y = this->y() + widgetTop->height();
			chatRecordDialog->move(x,y);
		}
	}
	else
	{
		QDialog::mouseMoveEvent(event);
	}
}

void SingleChatDialog::on_pushButtonClose_clicked()
{
	emit signalChatClose(this);
	chatRecordDialog->close();
	this->close();
}

void SingleChatDialog::on_pushButtonSendMessage_clicked()
{
	QByteArray byte;
//	qDebug()<<"~~~~~~~~"<<byte.size();
	initChatMessage(byte);
//	qDebug()<<"~~~~~0000"<<byte.size();
	emit signalSendChatMessage(byte);
	//应该先对数据进行处理,处理完之后才清零这两个变量
	textEditRecord->insertNickName_DateTime(nickName_Self, QDateTime::currentDateTime(), 0);
	textEditRecord->insertChatMessage(textEditChat->toHtml());
	writeChatLogDb(db,id_Friend,QDateTime::currentDateTime(),textEditChat->toPlainText(),textEditChat->toHtml(),1);	//写入聊天记录数据库——LY
	count = 0;
//	qDebug()<<"!!!!!!!!"<<imageDataByte.size();
	imageDataByte.clear();
//	qDebug()<<"00000"<<imageDataByte.size();
	textEditChat->clear();
	out->device()->seek(0);
//	qDebug()<<"就是这样子设置快捷键的";
	textEditChat->setFocus();
}

void SingleChatDialog::on_pushButtonFont_clicked()
{
	bool ok;
	font = QFontDialog::getFont( &ok, QFont( "Times", 12 ), this );
	if (ok)
	{
		qDebug()<< "font被设置为用户选择的字体";
	}
	else
	{
		qDebug() << "用户取消这个对话框，font被设置为初始值，在这里就是Times, 12";
	}
	textEditChat->setInsertTextFormat(font, color);
}

void SingleChatDialog::on_pushButtonColor_clicked()
{
	color = QColorDialog::getColor();
	textEditChat->setInsertTextFormat(font, color);
}

void SingleChatDialog::on_pushButtonPicture_clicked()
{
	QString imgPath = QFileDialog::getOpenFileName(0, "选择要发送的文件", "./temp", "图片 (*.png *.xpm *.jpg)");
	QPixmap imagePix = QPixmap(imgPath);
	if(imagePix.isNull())
	{
		qDebug()<<"图片加载失败";
	}
	else
	{
		++count;
		QByteArray imageByte;
		pictureToByte(imagePix, imageByte);
		slotAddImageData(imageByte);
		textEditChat->insertImage(imagePath);
	}
}


void SingleChatDialog::on_pushButtonIntercept_clicked()
{
	SnapScreen *snapScreen=new SnapScreen();
	snapScreen->show();
}



void SingleChatDialog::on_pushButtonRecord_clicked()
{
	if(chatRecordDialog->isHidden())
	{
		int x = this->x()+ this->width();
		int y = this->y() + widgetTop->height();
		chatRecordDialog->move(x,y);
		chatRecordDialog->show();
		chatRecordDialog->initData();
	}
	else
	{
		chatRecordDialog->hide();
	}
}

void SingleChatDialog::writeChatLogDb(QSqlDatabase chatDb,qint32 id,QDateTime time,QString text,QString html,int type)
{
		QSqlQuery query(chatDb);
		QString tablename ="chatlog"  + QString::number(id);

		QString sqltemp  = QString("insert into %1(chat_date,chat_text,chat_text_html,is_send) values( ?, ?, ?, ?)").arg(tablename);
		query.prepare(sqltemp);
		query.addBindValue(time);
		query.addBindValue(text);
		query.addBindValue(html);
		query.addBindValue(type);
		query.exec();
		//chatlog_id 不填写，让其自动增长，is_send为0表示接收到的信息，为1表发送
}


void SingleChatDialog::on_textEditRecord_textChanged()
{
	QTextCursor textCursor = textEditRecord->textCursor();
	textCursor.movePosition(QTextCursor::End);
	textEditRecord->setTextCursor(textCursor);
}

void SingleChatDialog::on_pushButtonFile_clicked()
{
	filePath = QFileDialog::getOpenFileName(this, "选择要打开的文件", ".", "*.*");
	fileName = filePath.right(filePath.size() - filePath.lastIndexOf('/') - 1);
	qDebug()<<"要传输的文件是"<<fileName;
	file = new QFile(filePath);
	if(!file->open(QFile::ReadOnly))
	{
		qDebug()<<"要传输的文件没有办法打开";
	}
	////////////////////////////////////////////////////
	//获取文件类型对应的图标
	getFileIconData();
	/////////////////////////////////////////////////////////
	totalSize = file->size();

	QByteArray bytes;
	QDataStream out(&bytes, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);

	out << qint32(0);
	out << qint32(33);
	out <<id_Self <<id_Friend << totalSize <<fileName << fileIconData;
	out.device()->seek(0);
	totalSize += bytes.size();
	out << qint32(bytes.size() - (qint32)(sizeof(qint64)));
	emit signalTransferFile(bytes);
}



void SingleChatDialog::on_pushButtonAbsorb_clicked()
{
	if(absorb == false)
	{//还没有显示那就显示出来
		widgetAbsorb->setHidden(false);
		slotShowColorValue();
		absorb = true;
	}
	else
	{
		widgetAbsorb->setHidden(true);
		absorb = false;
	}
}
