#include "ChatRecordDialog.h"
#include <QMessageBox>
#define PAGEITEM 50

ChatRecordDialog::ChatRecordDialog(QSqlDatabase chatDb, qint32 friend_id, qint32 self_id, QString friend_nick, QString self_nick, QWidget *parent) :
    db(chatDb),id_Friend(friend_id),id_Self(self_id),nickName_Friend(friend_nick),nickName_Self(self_nick),QDialog(parent)
{
    setupUi(this);
    initSet();
}

void ChatRecordDialog::on_pushButtonClose_clicked()
{
    this->hide();
}

void ChatRecordDialog::initSet()
{
    //用于设置无边框和显示位置
     this->setWindowFlags(Qt::FramelessWindowHint);
     this->setAttribute(Qt::WA_TranslucentBackground, true);
     pushButtonClose->setGeometry(410,5,10,10);//这里Y的是可以锁定的，但是X的不应该如此锁定，由于还没有修改窗口大小功能，可以先这么写
     //下面是关于dateWidget控件的设置
     dateWidget = new QCalendarWidget;
     dateWidget->setMinimumDate(QDate(2000,1,1));
     dateWidget->setMaximumDate(QDate::currentDate());
     dateWidget->setWindowTitle("Date");
     connect(dateWidget,SIGNAL(selectionChanged()),this,SLOT(slotSelectDate()));
}

void ChatRecordDialog::initData()
{
	/////////////////////////////////////////////
	labelDate->setText(QDate::currentDate().toString("yyyy-MM-dd"));
	QSqlQuery query(db);
	QString tableName = "chatlog" + QString::number(id_Friend);
	QString total = QString("select count(*) from %1").arg(tableName);
	query.exec(total);
	query.next();
	sum = query.value(0).toInt();
	if(sum != 0)
	{
		sum = sum / PAGEITEM + 1;
	}
	page = sum;
	if(sum== 0 || sum == 1)
	{
		//如果没有聊天记录的话
		lineEditPage->setDisabled(true);
		pushButtonFirstPage->setDisabled(true);
		pushButtonLeftPage->setDisabled(true);
		pushButtonDate->setDisabled(true);
	}
	pushButtonLastPage->setDisabled(true);
	pushButtonRightPage->setDisabled(true);
	labelPage->setText(QString::number(sum));
	lineEditPage->setText(QString::number(page));
	//以上是乱七八糟的东西的数值初始化
	//////////////////////////////////////////
	//下面是聊天记录文本的初始化
	if(sum != 0)
	{
		renovate();
	}
	//////////////////////////////////////////////////////
}

void ChatRecordDialog::on_pushButtonDate_clicked()
{
	if(dateWidget->isVisible())
	{
		dateWidget->close();
	}
	dateWidget->show();

	dateWidget->move(pushButtonDate->x(),widgetTool->y() - 160);
}

void ChatRecordDialog::slotSelectDate()
{
	labelDate->setText( dateWidget->selectedDate().toString("yyyy-MM-dd"));
	dateWidget->close();
	///////////////////////////////////
	//这里会有一段是根据所选第日期去查找数据库
	///////////////////////////////////
}

void ChatRecordDialog::on_textEdit_textChanged()
{
    QTextCursor textCursor = textEdit->textCursor();
    textCursor.movePosition(QTextCursor::End);
    textEdit->setTextCursor(textCursor);
}

void ChatRecordDialog::renovate()
{
	QSqlQuery query(db);
	QString tableName = "chatlog" + QString::number(id_Friend);
	textEdit->clear();

	//如果记录不为0
	int temp;
	QString dateTime;
	date = "";
	temp = (sum - page) * PAGEITEM;
	QString text = QString("select chat_date,chat_text_html,is_send,chatlog_id"
						   " from (select * from %1 order by chatlog_id desc"
						   " limit %2,50)as a order by chatlog_id asc").arg(tableName).arg(QString::number(temp));
	query.exec(text);
	while(query.next())
	{
		dateTime = query.value(0).toString();
		temp = dateTime.indexOf("T");
		if(date != dateTime.left(temp))
		{
			date = dateTime.left(temp);
			textEdit->append(date);
			textEdit->append("--------------------------------------------------------------");
		}
		time = dateTime.mid(temp + 1);
		html = query.value(1).toString();
		is_send = query.value(2).toBool();
		if(is_send)
		{
			textEdit->insertHtml(QString("<br /><span style=\" %1\">%2 %3</span><br />")
								 .arg(" color:#27ff0f;").arg(nickName_Self).arg(time));
		}
		else
		{
			textEdit->insertHtml(QString("<br /><span style=\" %1\">%2 %3</span><br />")
								 .arg(" color:#1d64ff;").arg(nickName_Friend).arg(time));
		}
		textEdit->insertHtml(html);
	}
}

void ChatRecordDialog::on_pushButtonFirstPage_clicked()
{
    //转到首页
    page = 1;
    lineEditPage->setText(QString::number(page));
    pushButtonFirstPage->setDisabled(true);
    pushButtonLeftPage->setDisabled(true);
    pushButtonRightPage->setDisabled(false);
    pushButtonLastPage->setDisabled(false);
    renovate();
}

void ChatRecordDialog::on_pushButtonLeftPage_clicked()
{
	//左边一页
		page -= 1;
		lineEditPage->setText(QString::number(page));
		if(page == 1)
		{
			pushButtonFirstPage->setDisabled(true);
			pushButtonLeftPage->setDisabled(true);
		}
		else
		{
			pushButtonFirstPage->setDisabled(false);
			pushButtonLeftPage->setDisabled(false);
		}
		pushButtonRightPage->setDisabled(false);
		pushButtonLastPage->setDisabled(false);
		renovate();
}

void ChatRecordDialog::on_pushButtonLastPage_clicked()
{
	//转到末页
	page = sum;
	lineEditPage->setText(QString::number(page));
	pushButtonFirstPage->setDisabled(false);
	pushButtonLeftPage->setDisabled(false);
	pushButtonRightPage->setDisabled(true);
	pushButtonLastPage->setDisabled(true);
	renovate();
}

void ChatRecordDialog::on_pushButtonRightPage_clicked()
{
	//右边一页
		page += 1;
		lineEditPage->setText(QString::number(page));
		if(page == sum)
		{
			pushButtonRightPage->setDisabled(true);
			pushButtonLastPage->setDisabled(true);
		}
		else
		{
			pushButtonRightPage->setDisabled(false);
			pushButtonLastPage->setDisabled(false);
		}
		pushButtonFirstPage->setDisabled(false);
		pushButtonLeftPage->setDisabled(false);
		renovate();
}
