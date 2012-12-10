#include "MyTextEdit.h"
#include <QBuffer>
#include <QDebug>
#include <QKeyEvent>
#include <QApplication>
#include <QClipboard>
#include <QTextCursor>
#include <QDateTime>
MyTextEdit::MyTextEdit(QWidget *parent) :
	QTextEdit(parent)
{
	initData();
}

void MyTextEdit::keyPressEvent(QKeyEvent *event)
{
	if(event->matches(QKeySequence::Paste))
	{//如果是粘贴组合键
		qDebug()<<"按下了粘贴组合键";
		QClipboard *clipboard = QApplication::clipboard();
		//还要进行类型的判断
		const QMimeData *mimeData = clipboard->mimeData();
		if(mimeData->hasImage())
		{
			qDebug()<<"剪贴板含有图片";
			//插入图片
			QPixmap tempPix = qvariant_cast<QPixmap>(mimeData->imageData());
			pastePicture(tempPix);//处理粘贴图片的函数
		}
		else if(mimeData->hasText())
		{
			qDebug()<<"剪贴板含有文字";
			this->setText(mimeData->text());
		}
		else if(mimeData->hasHtml())
		{
			qDebug()<<"剪贴板含有HTML";
			this->setHtml(mimeData->html());
		}
		else
		{
			qDebug()<<"剪贴板含有其他东西";
		}
	}
	/*
	else if(event->key() == Qt::Key_Backspace)
	{//按下的是退格键,
		QTextEdit::keyPressEvent(event);
		qDebug()<<"按下退格键";
	}
	*/
	else
	{//如果是其他情况
		qDebug()<<"按下了其他组合键";
		QTextEdit::keyPressEvent(event);
	}
}

void MyTextEdit::pastePicture(const QPixmap &tempPix)
{
	++count;
	QByteArray bytes;
	QBuffer buffer(&bytes);
	buffer.open(QIODevice::WriteOnly);
	tempPix.save(&buffer, "PNG"); //
	emit signalPastePicture(bytes);
	QString filePath =QString("./temp/%1.png").arg(QString::number(count));
	tempPix.save(filePath, "PNG");
	this->insertImage(filePath);
}

void MyTextEdit::initData()
{
	count = 100;
}

void MyTextEdit::setInsertTextFormat(QFont font, QColor color)
{//简单点说就是实现随意更改输入字体和颜色
	////  设置光标的选区，使格式作用于选区内的字符，若没有选区则作用于光标所在处的字符
	QTextCharFormat fmt;
	fmt.setForeground(color);
	fmt.setFont(font);
	QTextCursor cursor = textCursor();
	cursor.mergeCharFormat(fmt);
	mergeCurrentCharFormat(fmt);
}

void MyTextEdit::insertImage(const QString &path)
{
	this->insertHtml(QString("<img src=%1 />").arg(path));
}

void MyTextEdit::insertNickName_DateTime(const QString &nickName, const QDateTime &time, int type)
{
	QTextCursor cursor = this->textCursor();
	cursor.movePosition(QTextCursor::End);
	this->setTextCursor(cursor);

	if(MESSAGE_SELF == type)
	{//前后都加<br />分割
		this->insertHtml(QString("<br /><span style=\" %1\">%2 %3</span><br />")
						 .arg(" color:#27ff0f;").arg(nickName).arg(time.time().toString("hh:mm:ss")));
	}
	else if(MESSAGE_FRIEND == type)
	{
		this->insertHtml(QString("<br /><span style=\" %1\">%2 %3</span><br />")
						 .arg(" color:#1d64ff;").arg(nickName).arg(time.time().toString("hh:mm:ss")));
	}
	else
	{
		qDebug()<<"判断类型的时候出错拉";
		return;
	}
}

void MyTextEdit::insertChatMessage(const QString &html)
{
	this->insertHtml(html);
}
