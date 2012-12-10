#include "FriendInfoDetailDialog.h"
#include <QDebug>
#include <QMouseEvent>
#include <QApplication>
#include <QDesktopWidget>
FriendInfoDetailDialog::FriendInfoDetailDialog(PersonalInfoStruct &friendInfo,bool allowEdit, QWidget *parent) :
	QDialog(parent)
{
	setupUi(this);
	myFriend = friendInfo;
	updateGui(myFriend);
	initSetting();
	pushButtonDetailInfor->setEnabled(allowEdit);
}
void FriendInfoDetailDialog::updateGui(PersonalInfoStruct& info)
{
	myFriend=info;
	labelHeadImage->setPixmap(myFriend.headImagePixmap.scaled(labelHeadImage->size()));
	labelNickName->setText(myFriend.nickname);
	labelNickName->adjustSize();
	labelId->move(labelNickName->width()+labelNickName->x(), labelId->y());
	labelId->setText(QString::number(myFriend.userID));
	labelMark->setText(myFriend.mark);
	labelMood->setText(myFriend.mood);
	//这是性别的
	if(myFriend.sex == QString("M"))
	{
		labelSex_2->setText(QString::fromUtf8("男"));
	}
	else if(myFriend.sex == QString("F"))
	{
		labelSex_2->setText(QString::fromUtf8("女"));
	}
	else
	{
		labelSex_2->setText("春哥"+myFriend.sex);
	}
	//年龄= 当前年份 - 生日  这里只是粗略计算.没有把月份计算在内
	labelAge_2->setText(QString::number(QDate::currentDate().year() - myFriend.birthday.year()));
	labelConstellation_2->setText(QString::fromUtf8("功能尚未开放"));
	labelProvince_2->setText(QString::fromUtf8("功能尚未开放"));
	labelPhone_2->setText(QString::fromUtf8("功能尚未开放"));
	labelYear_2->setText(QString::fromUtf8("功能尚未开放"));
	labelBloodType_2->setText(QString::fromUtf8("功能尚未开放"));
	labelCity_2->setText(QString::fromUtf8("功能尚未开放"));
	labelMail_2->setText(myFriend.mail);
	//这个是修改部分的
	lnEditNickname->setText(myFriend.nickname);
	if(myFriend.sex==QString("M"))
	{
		cmbBoxSex->setCurrentIndex(0);
	}
	else if(myFriend.sex==QString("F"))
	{
		cmbBoxSex->setCurrentIndex(1);
	}
	else qDebug()<<"有个不男不女的字段";
	dtEditBirthday->setDate(myFriend.birthday);
	lnEditMail->setText(myFriend.mail);
	lnEditMood->setText(myFriend.mood);
	lblHeadImage->loadImage(myFriend.headImageByte);
}
void FriendInfoDetailDialog::initSetting()
{
	setAttribute(Qt::WA_DeleteOnClose);//以便释放内存
	////////////////////////////////////////////////////////////
	//这里以后用一个单独的函数
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	int x = (qApp->desktop()->width() - this->width()) / 2;
	int y = (qApp->desktop()->height() - this->height()) / 2;
	this->move(x, y);
	editing=false;
	///////////////////////////////////////////////////////////////
}

void FriendInfoDetailDialog::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_OffsetPosition  = event->globalPos() - frameGeometry().topLeft();//获取屏幕左上角坐标与软件左上角坐标的差
	}
	else
	{
		QDialog::mousePressEvent(event);
	}
}
void FriendInfoDetailDialog::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		move(event->globalPos() - m_OffsetPosition);
	}
	else
	{
		QDialog::mouseMoveEvent(event);
	}
}
void FriendInfoDetailDialog::closeEvent(QCloseEvent *event)
{
	emit signalClose(myFriend.userID);
}

void FriendInfoDetailDialog::on_pushButtonUpdate_clicked()
{
	if(pushButtonDetailInfor->isEnabled())
	{
		if(editing)//写个人信息
		{
			//把个人的新内容发出
			PersonalInfoStruct newInfo;
			newInfo.userID=myFriend.userID;
			newInfo.mark="";
			newInfo.nickname=lnEditNickname->text();
			if(cmbBoxSex->currentIndex()==0)
			{
				newInfo.sex=QString("M");
			}
			else if(cmbBoxSex->currentIndex()==1)
			{
				newInfo.sex=QString("F");
			}
			else
			{
				qDebug()<<"居然有不男不女的选项?!";
				newInfo.sex=QString("?");
			}
			newInfo.birthday=dtEditBirthday->date();
			newInfo.mail=lnEditMail->text();
			newInfo.mood=lnEditMood->text();
			newInfo.headImageByte=lblHeadImage->getImageDataByte();
			emit signalSetMyInfo(newInfo);
			qDebug()<<"更新个人信息的信号已经发出";
		}
		else//读个人信息
		{
			emit signalGetMyInfo();
			qDebug()<<"读取个人更新信息";
		}
	}
	else
	{//这个是其他人的
		emit signalUpdateFriendInfo(myFriend.userID);
		qDebug()<<"读取他人的更新信息";
	}
}
void FriendInfoDetailDialog::on_pushButtonClose_clicked()
{
	this->close();
}
void FriendInfoDetailDialog::on_pushButtonOk_clicked()
{
	this->close();
}
void FriendInfoDetailDialog::on_pushButtonBaseInfor_clicked()
{
	widgetPersonalInfor->raise();
	pushButtonBaseInfor->raise();
	pushButtonDetailInfor->raise();
	editing=false;
}
void FriendInfoDetailDialog::on_pushButtonDetailInfor_clicked()
{
	widgetPersonalInfor_allowEdit->raise();
	pushButtonBaseInfor->raise();
	pushButtonDetailInfor->raise();
	editing=true;
}