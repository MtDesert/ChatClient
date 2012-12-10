#ifndef PASSWDPROTECTFORM_H
#define PASSWDPROTECTFORM_H

#include "ui_PasswdProtectForm.h"
#include <QStringList>

class PasswdProtectForm : public QWidget, private Ui::PasswdProtectForm
{//这个类是密码保护的
	Q_OBJECT
	
public:
	explicit PasswdProtectForm(QWidget *parent = 0);
signals:
	void signalProtectinfor(QByteArray &inforList);
private slots:

	void on_pushButtonOK_clicked();

private:
	void initSetting();
	void initData();
	void getGuiData();
private:
	QString question1;	//问题还有答案
	QString question2;
	QString question3;
	QString answer1;
	QString answer2;
	QString answer3;
};

#endif // PASSWDPROTECTFORM_H
