#include "PasswdProtectForm.h"

PasswdProtectForm::PasswdProtectForm(QWidget *parent) :
	QWidget(parent)
{
	setupUi(this);
	initData();
	initSetting();
}

void PasswdProtectForm::initSetting()
{
	this->setWindowFlags(Qt::FramelessWindowHint);
}

void PasswdProtectForm::initData()
{
	question1 = "";
	question2 = "";
	question3 = "";
	answer1 = "";
	answer2 = "";
	answer3 = "";
}

void PasswdProtectForm::getGuiData()
{
	question1 = lineEditQuestion1->text();
	question2 = lineEditQuestion2->text();
	question3 = lineEditQuestion3->text();
	answer1 = lineEditAnswer1->text();
	answer2 = lineEditAnswer2->text();
	answer3 = lineEditAnswer3->text();
}

void PasswdProtectForm::on_pushButtonOK_clicked()
{
	getGuiData();	//获取所填写的数据
	QByteArray data;
	QDataStream out(&data, QIODevice::WriteOnly);
	out << question1<<question2<<question3<<answer1<<answer2<<answer3;
	emit signalProtectinfor(data);
}
