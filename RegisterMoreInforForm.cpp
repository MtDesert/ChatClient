#include "RegisterMoreInforForm.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
RegisterMoreInforForm::RegisterMoreInforForm(QDate date, QWidget *parent) :
	birthday(date), QWidget(parent)
{
	setupUi(this);
	initData();
	initGui();
	initSetting();
}

void RegisterMoreInforForm::initSetting()
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
}

void RegisterMoreInforForm::initData()
{
	animal << QString::fromUtf8("鼠") << QString::fromUtf8("牛") << QString::fromUtf8("虎") << QString::fromUtf8("兔")
		   << QString::fromUtf8("龙") << QString::fromUtf8("蛇") << QString::fromUtf8("马") << QString::fromUtf8("羊")
		   << QString::fromUtf8("猴") << QString::fromUtf8("鸡") << QString::fromUtf8("狗") << QString::fromUtf8("猪");
	nickName = "";
	animalYear = "";
	constellation = "";
	blood = "";
	province = "";
	city = "";
	phone = "";
	mail = "";
}

void RegisterMoreInforForm::initGui()
{
	imageLabel->setWordWrap(true);
	///////////////////////////////////////////////////
	//这里是计算生肖
	int y = (birthday.year() - ANIMAL_START) % 12;
	//qDebug()<<animal;
	qDebug()<<animal.value(y);
	labelYear2->setText(animal.value(y));
	/////////////////////////////////////////////
	//这里是计算星座
	int mon = birthday.month();
	int day = birthday.day();
	this->constellations(mon, day);
	labelConstellation2->setText(constellation);
	///////////////////////////////////////////
}

void RegisterMoreInforForm::getGuiData()
{
	nickName = lineEditNickName->text();
	animalYear = labelYear2->text();
	qDebug()<<animalYear;

	constellation = labelConstellation2->text();
	blood = comboBoxBlood->currentText();
	province = comboBoxProvince->currentText();
	city = comboBoxCity->currentText();
	phone = lineEditPhone->text();
	mail = lineEditMail->text();
	//regInforList.clear();
	//regInforList << nickName << year << constellation << blood << province << city << phone << mail;
	headImageByte = imageLabel->getImageDataByte();
	headImagePix.loadFromData(headImageByte, "JPEG");
}
//判断星座的函数,很没技术含量
void RegisterMoreInforForm::constellations(int month, int day)
{
	if (month == 1 && day >= 20 || month == 2 && day <= 18)
	{
		constellation = QString::fromUtf8("水瓶座");
		return;
	}
	if (month == 2 && day >= 19 || month == 3 && day <= 20)
	{
		constellation = QString::fromUtf8("双鱼座");
		return;
	}
	if (month == 3 && day >= 21 || month == 4 && day <= 19)
	{
		constellation = QString::fromUtf8("白羊座");
		return;
	}
	if (month == 4 && day >= 20 || month == 5 && day <= 20)
	{
		constellation = QString::fromUtf8("金牛座");
		return;
	}
	if (month == 5 && day >= 21 || month == 6 && day <= 21)
	{
		constellation = QString::fromUtf8("双子座");
		return;
	}
	if (month == 6 && day >= 22 || month == 7 && day <= 22)
	{
		constellation = QString::fromUtf8("巨蟹座");
		return;
	}
	if (month == 7 && day >= 23 || month == 8 && day <= 22)
	{
		constellation = QString::fromUtf8("狮子座");
		return;
	}
	if (month == 8 && day >= 23 || month == 9 && day <= 22)
	{
		constellation = QString::fromUtf8("处女座");
		return;
	}
	if (month == 9 && day >= 23 || month == 10 && day <= 22)
	{
		constellation = QString::fromUtf8("天秤座");
		return;
	}
	if (month == 10 && day >= 23 || month == 11 && day <= 21)
	{
		constellation = QString::fromUtf8("天蝎座");
		return;
	}
	if (month == 11 && day >= 22 || month == 12 && day <= 21)
	{
		constellation = QString::fromUtf8("射手座");
		return;
	}
	if (month == 12 && day >= 22 || month == 1 && day <= 19)
	{
		constellation = QString::fromUtf8("摩羯座");
		return;
	}
	qDebug()<<constellation;
}

void RegisterMoreInforForm::on_pushButtonPasswdProtect_clicked()
{//按下密码保护按键的时候就发信号通知RegisterDialog滑出密码保护界面
	emit signalShowProcect();
}

void RegisterMoreInforForm::on_pushButtonOK_clicked()
{
	getGuiData();
	QByteArray data;
	QDataStream out(&data, QIODevice::WriteOnly);
	out  << nickName << animalYear << constellation << blood << province << city << phone << mail << headImageByte;
	emit signalRegInfor(data);
}

