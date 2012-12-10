#include "RegisterDialog.h"
#include <QMessageBox>
#include <QBuffer>
#include <QMouseEvent>
#include <QLayout>
#include <QDataStream>
#include <QRegExp>
#include <QDebug>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QCryptographicHash>
#include <QTimer>
#include <qmath.h>
RegisterDialog::RegisterDialog(QWidget *parent) :
	QDialog(parent)
{
	setupUi(this);
	initData();
	initGui();
	initSetting();
	initNetWork();
}

void RegisterDialog::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		pointOffset = event->globalPos() - frameGeometry().topLeft();
	}
	else
	{
		QDialog::mousePressEvent(event);
	}
}

void RegisterDialog::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		move(event->globalPos() - pointOffset);
		if(NULL != regMoreInforForm)
		{
			int y = this->y() + OFFSET; //加31像素的话就证号从titelBar的下面出来
			regMoreInforForm->move(this->x() + this->width(), y);
		}
		if(NULL != passwdProtectForm)
		{
			int x = regMoreInforForm->x() + regMoreInforForm->width() - passwdProtectForm->width();
			int y = regMoreInforForm->y() + regMoreInforForm->height();
			passwdProtectForm->move(x, y);
		}
	}
	else
	{
		QDialog::mouseMoveEvent(event);
	}
}

void RegisterDialog::isCanRegister()
{//这个函数是判断注册按钮是否可以按下的
	if(name.length() && passwd.length() && sex.length())
	{
		if(lineEditPasswd->text() == lineEditPasswdConfirm->text())
		{
			pushButtonRegister->setEnabled(true);
			labelTip->setText(QString::fromUtf8("注册信息已填写完整,可以注册"));
		}
		else
		{
			pushButtonRegister->setEnabled(false);
			labelTip->setText(QString::fromUtf8("注册信息还未填写完整,请补完!"));
		}
	}
	else
	{
		pushButtonRegister->setEnabled(false);
		labelTip->setText(QString::fromUtf8("注册信息还未填写完整,请补完!"));
	}
}


void RegisterDialog::resultForRegister(const QString &str)
{
	int pressed = QMessageBox::information(0, QString::fromUtf8("注册结果"), QString::fromUtf8(str.toAscii().data()), QMessageBox::Ok, QMessageBox::Cancel);
	if(QMessageBox::Ok == pressed)
	{//如果用户选择OK则保留当前注册界面

	}
	else
	{//如果用户选择cancel则关闭注册界面
		this->close();
	}
}

void RegisterDialog::on_pushButtonClose_clicked()
{
	if(NULL != regMoreInforForm)
	{
		regMoreInforForm->close();
	}
	if(NULL != passwdProtectForm)
	{
		passwdProtectForm->close();
	}
	close();
}

void RegisterDialog::on_pushButtonRegister_clicked()
{//注册按钮被按下
	registerTcpSocket->connectToHost(QHostAddress("127.0.0.1"),8888);
}

void RegisterDialog::on_lineEditPasswd_textEdited(const QString &arg1)
{
	passwd = arg1;
	passwdConfirm = lineEditPasswdConfirm->text();

	passwdMatch(passwd, passwdConfirm);
	isCanRegister();
}

void RegisterDialog::on_lineEditPasswdConfirm_textEdited(const QString &arg1)
{
	passwd = lineEditPasswd->text();
	passwdConfirm = arg1;
	passwdMatch(passwd, passwdConfirm);
	isCanRegister();
}

void RegisterDialog::passwdMatch(const QString &passwd, const QString &passwdConfirm)
{
	if(passwd.length() > 0 && passwdConfirm.length() > 0)
	{//说明了密码还有密码确认都是有值的
		if(passwd != passwdConfirm)
		{
			qDebug()<<"no match"<<endl;
			labelPasswdError->setText(QString::fromUtf8("两次输入的密码不匹配!"));
		}
		else
		{
			qDebug()<<"match two string!"<<endl;
			labelPasswdError->setText(QString::fromUtf8("两次输入的密码匹配!"));
		}
	}
	else
	{
		if(passwd.length() > 0)
		{
			passPart(passwd);
		}
		else if(passwdConfirm.length() > 0)
		{
			passPart(passwdConfirm);
		}
	}
}

void RegisterDialog::slotSelectDate()
{
	birthday = dataWidget->selectedDate();
	labelDate->setText(birthday.toString("yyyy.MM.dd"));
	dataWidget->close();
	isCanRegister();
}

void RegisterDialog::on_lineEditName_textChanged(const QString &arg1)
{
	name = arg1;
	isCanRegister();
}

void RegisterDialog::on_radioMan_clicked()
{
	sex = "M";
	isCanRegister();
}

void RegisterDialog::on_radioWomem_clicked()
{
	sex = "F";
	isCanRegister();
}

void RegisterDialog::slotReadUdpDatagramFromServer()
{
	qDebug()<<"readUdpDatagramFromServer"<<endl;
	QString strRegisterResult;
	while(registerUdpSocket->hasPendingDatagrams())
	{
		QByteArray data;
		data.resize(registerUdpSocket->size());
		registerUdpSocket->readDatagram(data.data(), data.size());
		strRegisterResult = data.data();
	}
	//////根据服务端的返回信息做下一步回应
	//
	if(strRegisterResult == QString("REGISTER_SUCCESS"))
	{//注册成功则提示注册成功,并询问用户是否继续注册
		resultForRegister(QString("注册成功,是否继续注册?"));
	}
	else if(strRegisterResult == QString("REGISTER_FALSE"))
	{//注册成功则提示注册成功,并询问用户是否继续注册
		resultForRegister(QString("注册失败,该账户名已经被占用! 是否继续注册?"));
	}
}

void RegisterDialog::slotConnectToServerReady()
{
	QByteArray bytes;
	QDataStream out(&bytes, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);

	out << (qint32)0;
	out << (qint32)1;
	out << (qint32)0;
	passwd = QCryptographicHash::hash(passwd.toAscii(), QCryptographicHash::Md5).toHex();
	//qDebug()<<QString::fromUtf8("注册时候的密码MD5加密值是:")<<passwd<<endl;
	out << name << nickName << passwd  << sex << birthday << mail
		<< question1 << question2 << question3 << answer1 << answer2 << answer3;
	///////下面是图片存储传送
	out << headImageByte;
	///////////////////////////////
	//好吧把后面的数据添加在图片的后面
	out << animalYears << constellation << blood << province << city << phone;

	//////////////////////////////////
	out.device()->seek(0);
	//out << (qint32)(bytes.size() - (int)sizeof(QString::number(0,10)));
	out << (qint32)(bytes.size() - (int)sizeof(qint32));
	registerTcpSocket->write(bytes);
	qDebug()<<QString::fromUtf8("client send message to server !! ");

	//this->close();
}

void RegisterDialog::slotProtectInfor(QByteArray &data)
{//收回密保界面的槽
	qDebug()<<QString::fromUtf8("收到密码保护的信号,现在进行处理");
	QDataStream in(&data, QIODevice::ReadOnly);
	in >> question1 >> question2 >> question3 >> answer1 >> answer2 >> answer3;
	//qDebug()<<question1<<question2<<question3<<answer1<<answer2<<answer3<<endl;
	//nice之后就开始让密保界面缩回去
	if(NULL != passwdProtectForm && NULL != regMoreInforForm)
	{//详细界面和密保界面都要存在
		QPoint end;
		end.setX(passwdProtectForm->x());
		end.setY(passwdProtectForm->y() - passwdProtectForm->height());
		moveAnimation(passwdProtectForm, passwdProtectForm->pos(), end);
		QTimer::singleShot(800, this, SLOT(slotCloseProtectForm()));
	}
	else
	{
		qDebug()<<QString::fromUtf8("不满足密保界面回收条件");
	}
}

void RegisterDialog::slotShowProtectInfor()
{//弹出密保界面的槽
	if(NULL != regMoreInforForm && NULL == passwdProtectForm)
	{//    详细信息界面存在            且                     密保界面不存在
		passwdProtectForm = new PasswdProtectForm;
		connect(passwdProtectForm, SIGNAL(signalProtectinfor(QByteArray&)), this, SLOT(slotProtectInfor(QByteArray&)));
		QPoint start;
		start.setX(regMoreInforForm->x() + regMoreInforForm->width() - passwdProtectForm->width());
		start.setY(regMoreInforForm->y());
		passwdProtectForm->show();
		QPoint end;
		end.setX(start.x());
		end.setY(regMoreInforForm->y() + regMoreInforForm->height());
		moveAnimation(passwdProtectForm, start, end);
	}
	else
	{
		qDebug()<<QString::fromUtf8("不满足密保界面弹出条件");
	}
}

void RegisterDialog::slotCloseProtectForm()
{//计时器超时响应
	//完成密保界面的关闭
	if(NULL != passwdProtectForm)
	{
		passwdProtectForm->close();
	}
	passwdProtectForm = NULL;
}

void RegisterDialog::slotMoreInfor(QByteArray &data)
{
	///先处理详细界面发来的详细注册信息
	QDataStream in(&data, QIODevice::ReadOnly);

	in >> nickName >> animalYears >> constellation >> blood >> province >> city >> phone >> mail >> headImageByte;
	headImagePix.loadFromData(headImageByte, "JPEG");
	qDebug()<<QString::fromUtf8("验证以下数据先");
	qDebug()<< nickName << animalYears << constellation << blood << province << city << phone << mail;
	//在处理详细界面收回的问题
	if(NULL != regMoreInforForm)
	{//如果详细界面存在才可以收回
		QPoint end;
		end.setX(regMoreInforForm->x() - regMoreInforForm->width());
		end.setY(regMoreInforForm->y());
		moveAnimation(regMoreInforForm, regMoreInforForm->pos(), end);
		QTimer::singleShot(800, this, SLOT(slotCloseRegMoreForm()));
	}
}

void RegisterDialog::slotCloseRegMoreForm()
{
	if(NULL != regMoreInforForm)
	{
		regMoreInforForm->close();
	}
	regMoreInforForm = NULL;
}

void RegisterDialog::passPlexity(int result, int length)
{//恩阿...这里如此繁杂...有时间我应该把这里改成表驱动
	switch(result)
	{
		case EXITS_NUMBER:
		{//1 要#include <qmath.h>
			allTime = pow(10, length);
			passwdConstTime();
			break;
		}
		case EXITS_LOWER:
		{//2
			allTime = pow(26, length);
			passwdConstTime();
			break;
		}
		case EXITS_NUMBER_LOWER:
		{//3
			allTime = pow((10+26), length);
			passwdConstTime();
			break;
		}
		case EXITS_UPPER:
		{//4
			allTime = pow(26, length);
			passwdConstTime();
			break;
		}
		case EXITS_NUMBER_UPPER:
		{//5
			allTime = pow((10+26), length);
			passwdConstTime();
			break;
		}
		case EXITS_LOWER_UPPER:
		{//6
			allTime = pow((26+26), length);
			passwdConstTime();
			break;
		}
		case EXITS_NUMBER_LOWER_UPPER:
		{//7

			allTime = pow((10+26+26), length);
			passwdConstTime();
			break;
		}
		case EXITS_SIGN:
		{//8

			allTime = pow(32, length);
			passwdConstTime();
			break;
		}
		case EXITS_NUMBER_SIGN:
		{//9

			allTime = pow((10+32), length);
			passwdConstTime();
			break;
		}
		case EXITS_LOWER_SIGN:
		{//10
			allTime = pow((26+32), length);
			passwdConstTime();
			break;
		}
		case EXITS_NUMBER_LOWER_SIGN:
		{//11


			allTime = pow((10+26+32), length);
			passwdConstTime();
			break;
		}
		case EXITS_UPPER_SIGN:
		{//12

			allTime = pow((26+32), length);
			passwdConstTime();
			break;
		}
		case EXITS_NUMBER_UPPER_SIGN:
		{//13

			allTime = pow((10+26+32), length);
			passwdConstTime();
			break;
		}
		case EXITS_LOWER_UPPER_SIGN:
		{//14

			allTime = pow((26+26+32), length);
			passwdConstTime();
			break;
		}
		case EXITS_NUMBER_LOWER_UPPER_SIGN:
		{//15
			allTime = pow((10+26+26+32), length);
			passwdConstTime();
			break;
		}
		default:
		{
			//		qDebug()<<QString::fromUtf8("在分析成分的时候出现了其他的情况");
		}
	}
	//	qDebug()<<"allTime "<<allTime;
}

void RegisterDialog::passwdConstTime()
{
	if(allTime > 63072000000000)
	{
		qDebug()<<QString::fromUtf8("人类已经无法破解你的密码了~~");
		labelPasswdComplex->setText(QString::fromUtf8("人类已经无法破解你的密码了~~"));
	}
	else
	{
		year = allTime / YEAR;
		mon = allTime % YEAR / MONTH;
		day = allTime % YEAR % MONTH / DAY;
		hour = allTime % YEAR % MONTH% DAY / HOUR;
		min = allTime % YEAR % MONTH% DAY % HOUR / MINUTE;
		sec = allTime % YEAR % MONTH% DAY % HOUR % MINUTE / 20000;
		labelPasswdComplex->setText(QString::fromUtf8("穷举此密码大概需要耗时%1年%2月%3天%4小时%5分钟%6秒")
									.arg(QString::number(year)).arg(QString::number(mon)).arg(QString::number(day)).arg(QString::number(hour))
									.arg(QString::number(min)).arg(QString::number(sec)));
	}
}

void RegisterDialog::passPart(const QString &str)
{
	int result = 0;
	for(int i = 0; i < str.length(); ++i)
	{
		qDebug()<<"p["<<i<<"] = "<<str[i];
		if(str[i].isDigit())
		{
			qDebug()<<"number";
			result |= EXITS_NUMBER;
		}
		else if(str[i].isLower())
		{
			qDebug()<<"lower";
			result |= EXITS_LOWER;
		}
		else if(str[i].isUpper())
		{
			qDebug()<<"upper";
			result |= EXITS_UPPER;
		}
		else
		{
			qDebug()<<"sign";
			result |= EXITS_SIGN;
		}
	}
	qDebug()<<result;
	//分析完组成成分之后就可以计算破解所需的理论耗时了
	passPlexity(result, str.length());
}

void RegisterDialog::initGui()
{
	lineEditPasswd->setEchoMode(QLineEdit::Password);
	lineEditPasswdConfirm->setEchoMode(QLineEdit::Password);
	pushButtonRegister->setEnabled(false);
	labelPasswdError->setText("");
	dataWidget = new QCalendarWidget;
	labelPasswdComplex->setWordWrap(true);
}

void RegisterDialog::initNetWork()
{
	registerUdpSocket = new QUdpSocket(this);
	if(!(registerUdpSocket->bind(8000, QUdpSocket::ShareAddress)))
	{
		qDebug()<<"register dialog has error"<<endl;
		qDebug()<<registerUdpSocket->errorString()<<endl;
		return;
	}
	connect(registerUdpSocket, SIGNAL(readyRead()), this, SLOT(slotReadUdpDatagramFromServer()));
	registerTcpSocket = new QTcpSocket(this);
	connect(registerTcpSocket, SIGNAL(connected()), this, SLOT(slotConnectToServerReady()));
	connect(dataWidget, SIGNAL(selectionChanged()), this, SLOT(slotSelectDate()));
}

void RegisterDialog::initData()
{
	regMoreInforForm = NULL;
	passwdProtectForm = NULL;
	///////////////////////////
	nickName = "";
	animalYears = "";
	constellation = "";
	blood = "";
	province = "";
	city = "";
	phone = "";
	mail = "";
	headImagePix = QPixmap();
	///////////////////////////
	question1 = "";
	question2 = "";
	question3 = "";
	answer1 = "";
	answer2 = "";
	answer3 = "";
}

void RegisterDialog::initSetting()
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->move(0, 0);
}

void RegisterDialog::moveAnimation(QWidget *widget, QPoint start, QPoint end)
{
	if(widget == regMoreInforForm)
	{
		this->raise();
	}
	else if(widget == passwdProtectForm)
	{
		regMoreInforForm->raise();
	}
	else
	{
		qDebug()<<QString::fromUtf8("判断widget是regMoreInforForm还是passwdProtectForm出现了问题");
	}
	QPropertyAnimation *animation = new QPropertyAnimation(widget, "pos");
	animation->setDuration(600);	//时间
	animation->setStartValue(start);
	animation->setEndValue(end);
	//animation->setEasingCurve(QEasingCurve::OutBounce);  //这里是变化的形式
	animation->start();
}

void RegisterDialog::on_pushButtonSetDate_clicked()
{
	dataWidget->setMinimumDate(QDate(1900, 1, 1));
	dataWidget->setMaximumDate(QDate::currentDate());
	dataWidget->show();
}

void RegisterDialog::on_pushButtonPasswdProcect_clicked()
{//这里要弹出详细界面了
	if(NULL == regMoreInforForm)
	{
		regMoreInforForm = new RegisterMoreInforForm(birthday);
		connect(regMoreInforForm, SIGNAL(signalShowProcect()), this, SLOT(slotShowProtectInfor()));
		connect(regMoreInforForm, SIGNAL(signalRegInfor(QByteArray&)), this, SLOT(slotMoreInfor(QByteArray&)));
		regMoreInforForm->show();
		QPoint start;
		start.setX(this->x() + this->width() - regMoreInforForm->width());
		start.setY(this->y() + OFFSET);
		QPoint end;
		end.setX(this->x()+ this->width());
		end.setY(start.y());
		moveAnimation(regMoreInforForm, start, end);
	}
	else
	{
		qDebug()<<"NULL != regMoreInforForm";
	}
}
