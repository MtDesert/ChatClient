#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include "ui_RegisterDialog.h"
#include "PasswdProtectForm.h"
#include "RegisterMoreInforForm.h"
#include <QCalendarWidget>
#include <QPropertyAnimation>
#include <QEasingCurve>
const int OFFSET = 30;	//弹出详细界面的偏移,为了正好从Titel下面出来
/////////////////////////////////////////////////
//这里定义的一大串东西是定义分析字符串得出的结果
const int EXITS_NUMBER = 0x1;
const int EXITS_LOWER = 0x2;
const int EXITS_UPPER = 0x4;
const int EXITS_SIGN = 0x8;
const int EXITS_NUMBER_LOWER = 0x3;
const int EXITS_NUMBER_UPPER = 0x5;
const int EXITS_LOWER_UPPER = 0x6;
const int EXITS_NUMBER_LOWER_UPPER = 0x7;
const int EXITS_NUMBER_SIGN = 0x9;
const int EXITS_LOWER_SIGN = 0xA;
const int EXITS_NUMBER_LOWER_SIGN = 0xB;
const int EXITS_UPPER_SIGN = 0xC;
const int EXITS_NUMBER_UPPER_SIGN = 0xD;
const int EXITS_LOWER_UPPER_SIGN = 0xE;
const int EXITS_NUMBER_LOWER_UPPER_SIGN = 0xF;
/////////////////////////////////////////////////
//和秒数有关的东西
const int MINUTE = 1200000;
const int HOUR = 72000000;
const int DAY = 1728000000;
const long WEEK = 12096000000;
const long MONTH  = 52560000000;
const qint64 YEAR = 630720000000;

/////////////////////////////////////////////////
class QMouseEvent;
class QRegExp;
class QUdpSocket;
class QTcpSocket;

class RegisterDialog : public QDialog, private Ui::RegisterDialog
{
	Q_OBJECT

public:
	explicit RegisterDialog(QWidget *parent = 0);

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

	void isCanRegister();   //判断注册按钮是否符合按下条件
	void resultForRegister(const QString &str);
private slots:
	void on_pushButtonClose_clicked(); //关闭按钮

	void on_pushButtonRegister_clicked(); //注册按钮

	void on_lineEditPasswd_textEdited(const QString &arg1);	//密码

	void on_lineEditPasswdConfirm_textEdited(const QString &arg1);	//密码确认

	void on_lineEditName_textChanged(const QString &arg1);	//编辑帐户名

	void on_pushButtonSetDate_clicked();	//按下设置生日

	void on_radioMan_clicked();		//男

	void on_radioWomem_clicked();//女

	void slotSelectDate();	//选定日期

	void slotReadUdpDatagramFromServer();

	void slotConnectToServerReady();

	void slotProtectInfor(QByteArray &data);	//收回密保界面

	void slotShowProtectInfor();	//把密保界面弹出

	void slotCloseProtectForm();//定时器超时响应槽 用于关闭密保界面,因为动画是非阻塞的

	void slotMoreInfor(QByteArray &data);	//收回详细注册信息界面

	void slotCloseRegMoreForm();
	void on_pushButtonPasswdProcect_clicked();

private:
	//好吧有时间我要把这里抽象成一个类...仍在这里太多了
	void passwdMatch(const QString &passwd, const QString &passwdConfirm); //比较两次输入的密码是否一致
	void passPart(const QString &str);//用于分析密码的组成成分 数字,字母(大小写),特殊符号
	void passPlexity(int result, int length);//用于分析破解密码所需要的理论时间
	void passwdConstTime();
	void initGui();
	void initNetWork();
	void initData();
	void initSetting();
	//移动的动画 第一个参数是要动的部件, 第二个是起点, 第三个是终点
	void moveAnimation(QWidget *widget, QPoint start, QPoint end);
private:
	QPoint pointOffset;
	////////基本注册信息变量
	QString name;
	QString passwd;
	QString passwdConfirm;
	QDate birthday;
	QString sex;
	QPixmap *img;
	QCalendarWidget *dataWidget;	//这个是日期控件
	////////////////////////////////////
	//详细注册信息有的变量
	RegisterMoreInforForm *regMoreInforForm;
	QString nickName;
	QString animalYears;
	QString constellation;
	QString blood;
	QString province;
	QString city;
	QString phone;
	QString mail;
	QPixmap headImagePix;
	QByteArray headImageByte;

	///////////////////////////////////
	//下面是密保相关的东西
	PasswdProtectForm *passwdProtectForm;
	QString question1;
	QString question2;
	QString question3;
	QString answer1;
	QString answer2;
	QString answer3;
	////////////////////////////////////////
	/////////////////////////////////一下是网络传输的
	//现在的设想是字符串都用UDP传输,涉及图片和大数据采用TCP传输
	QTcpSocket *registerTcpSocket;
	QUdpSocket *registerUdpSocket;
	////////////////////////////////////////
	//计算时间破解复杂度相关的变量
	long year;
	int mon;
	int day;
	int hour;
	int min;
	int sec;
	qint64 allTime;
	////////////////////////////////////////
};

#endif // REGISTERDIALOG_H
