#include "LoginDialog.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QBuffer>
#include <QMouseEvent>
#include <QDebug>
#include <QSqlError>
#include <QDir>
#include <QCompleter>
#include <QCryptographicHash>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
	QDialog(parent)
{
	setupUi(this);
	pushButtonLogin->setEnabled(false);
	comboBoxName->setEditable(true);
	pushButtonLogin->setEnabled(false);
	getCompleteName();//填充补完的列表的项
	tcpSocket = new QTcpSocket(this);
		tcpSocket->connectToHost(QHostAddress("127.0.0.1"), 8888);
	connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnectToHostReady()));
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadTcpDatagramFromServer()));
	initSetting();
}

void LoginDialog::mousePressEvent(QMouseEvent *event)
{//重载的鼠标按下
	if (event->button() == Qt::LeftButton)
	{
		m_OffsetPosition  = event->globalPos() - frameGeometry().topLeft();//获取屏幕左上角坐标与软件左上角坐标的差
	}
	else
	{
		QDialog::mousePressEvent(event);
	}
}

void LoginDialog::mouseMoveEvent(QMouseEvent *event)
{//重载的鼠标拖动
	if (event->buttons() == Qt::LeftButton)
	{
		move(event->globalPos() - m_OffsetPosition);
	}
	else
	{
		QDialog::mouseMoveEvent(event);
	}
}

void LoginDialog::on_pushButtonClose_clicked()
{
	close();
}

void LoginDialog::getCompleteName()
{//遍历user目录,因为每一个文件夹的名字都是以账户名命名,所以文件名就是自动补完的列表项
	QDir userDir("./user");			//指定目录为user因为其下有登录名名命名的文件夹
	userDir.setFilter(QDir::Dirs);	//只需要文件夹
	userDir.setSorting(QDir::Name);	//排序,可有可无

	fileNameList = userDir.entryInfoList();	//这里
	QFileInfoList::const_iterator iter;
	iter = fileNameList.begin();
	while(iter != fileNameList.end())
	{
		if((*iter).fileName() !="." && (*iter).fileName() !="..")
		{
			strFileNameList<<(*iter).fileName();
		}
		++iter;
	}

    completer = new QCompleter(strFileNameList, this);	//这里是重点QCompleter可以实现补完
	comboBoxName->setEditable(true);
	comboBoxName->addItems(list);
	comboBoxName->setCompleter(completer);
}

void LoginDialog::on_comboBoxName_textChanged(const QString &arg1)
{//用户输入信息时
	userName_login = arg1;
	autoFull(userName_login);
	if(userName_login.length() > 0 && passwd_login.length() > 0)
	{//帐号和密码都不能为空的时候才能点击
		pushButtonLogin->setEnabled(true);
	}
	else
	{
		pushButtonLogin->setEnabled(false);
	}
}

void LoginDialog::on_lineEditPasswd_textChanged(const QString &arg1)
{
	passwd_login = arg1;
	if(userName_login.length() > 0 && passwd_login.length() > 0)
	{//帐号和密码都不能为空的时候才能点击
		pushButtonLogin->setEnabled(true);
	}
	else
	{
		pushButtonLogin->setEnabled(false);
	}
}

void LoginDialog::on_pushButtonLogin_clicked()
{//登录按钮被按下
	state = comboBoxState->currentIndex();
	////////MD5
	//好吧现在要改成一次一密,pass+时间
	//第一次加密普通明文->MD5
	passwdHash = QCryptographicHash::hash(passwd_login.toAscii(), QCryptographicHash::Md5).toHex();
//	qDebug()<<passwdHash<<endl;
	//hash+盐
	QTime loginTime = QTime::currentTime();
	passwdHash += loginTime.toString();
//	qDebug()<<"passwdHash+time = "<<passwdHash<<endl;
	passwdHash = QCryptographicHash::hash(passwdHash.toAscii(), QCryptographicHash::Md5).toHex();
//	qDebug()<<QString::fromUtf8("Hash+盐之后的加密结果是")<<passwdHash<<endl;
	//占位,这里是网络验证部分////////////////////////
	//QByteArray datagram = userName_login.toAscii();
	QByteArray datagram;
	QDataStream out(&datagram, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << (qint32)0;	//空间站位
	out << (qint32)3;	//源ID
	out << (qint32)0;	//目的ID
	out << userName_login;
	out << loginTime;
	out << passwdHash;
	//如果用户更改过头像,那么imageLabel就会有数据
	QPixmap headImage;
	headImage.loadFromData(imageLabel->getImageDataByte());
	if(!headImage.isNull())
	{
		//qDebug()<<QString::fromUtf8("ImageLable传来有效的头像数据");
		QByteArray imageData;
		QBuffer buff(&imageData);
		headImage.save(&buff, "PNG");
		out << imageData;
	}
	out.device()->seek(0);
	out << (qint32)(datagram.size() - (qint32)sizeof(qint32));
	tcpSocket->write(datagram);
}

void LoginDialog::getUserInfoString()
{
	personInfo.clear();
	personInfo << userName_login;
	if(autoLogin)
	{
		personInfo << "true";
	}
	else
	{
		personInfo << "false";
	}
	if(rememberPasswd)
	{
		personInfo << "true";
	}
	else
	{
		personInfo << "false";
	}
	personInfo << passwd_login;
	personInfo << QString::number(state,10);
}

void LoginDialog::writePersonInfoIntoLocal()
{//嗯这个函数顾名思义是把登录时候的个人输入的信息记录到本地文件夹的函数
	QDir *dir = new QDir(".");
	if(!dir->exists("user"))
	{
		dir->mkdir("user");
	}
	dir->cd("user");

	if(!dir->exists(userName_login))
	{
//		qDebug()<<QString::fromUtf8("不存在用户信息文件夹,现在创建一个新的");
		dir->mkdir(userName_login);
	}
	dir->cd(userName_login);
	/////////////////////////////////////////////////////////
	///这里是保存当前登录时候的头像到本地的
	QString pixFormPath = imageLabel->GetImagePath();		//从imageLabel这里取得头像的图片数据
//	qDebug()<<pixFormPath<<endl;
//	qDebug()<<"dir.path()"<<dir->path()<<endl;
	QPixmap currentHeadImage(pixFormPath);
	QString pixSavePath = dir->path() + "/image";
//	qDebug()<<"pixSavePath"<<pixSavePath<<endl;
	currentHeadImage.save(pixSavePath, "PNG");
	QString fileName = "personal.xml";
	p_dir = dir;

	xmlWriter.writeInfo(dir->path(), fileName, personInfo);
	xmlWriter.writeInfo(dir->path(), QString("friend.xml"), personInfo);
}


void LoginDialog::autoFull(const QString &str)
{
	QString currentUserName = str;
	QFileInfoList::const_iterator iterName = fileNameList.begin();
	QString path = "";
	while(iterName != fileNameList.end())
	{
		if(currentUserName == (*iterName).fileName())
		{
			path += "./user/";
			path += currentUserName;
			path += "/personal.xml";
		}
		++iterName;
	}
	//qDebug()<<path<<endl;
	if(path.length() > 0)
	{
		xmlReader.Foreach(path);
		QString readRememberPasswd = xmlReader.getRemeberPasswd();
	//	qDebug()<<"readRememberPasswd"<<readRememberPasswd;
		//若记住密码,把密码和记住密码设置好.
		if("true" == readRememberPasswd)
		{
			QString readPasswd = xmlReader.getpasswd();
			lineEditPasswd->setEchoMode(QLineEdit::Password);
			lineEditPasswd->setText(readPasswd);
			checkBoxRemember->setChecked(true);
		}
		else
		{
			checkBoxRemember->setChecked(false);
			lineEditPasswd->clear();
		}

		QString readAtuoLogin = xmlReader.getAtuoLogin();
		if("true" == readAtuoLogin)
		{
			checkBoxAutoLogin->setChecked(true);
		}
		else
		{
			checkBoxAutoLogin->setChecked(false);
		}
		comboBoxState->setCurrentIndex(xmlReader.getState().toInt());

		QString pixmapPath = "./user/";
		pixmapPath += currentUserName;
		pixmapPath += "/image";
	//	qDebug()<<pixmapPath<<endl;
		QPixmap pixmap(pixmapPath);
		//imageLabel->setPixmap(pixmap.scaled(imageLabel->size()));
		imageLabel->loadImage(pixmap);
	}
	else
	{
		lineEditPasswd->clear();
		comboBoxState->setCurrentIndex(0);
		checkBoxRemember->setChecked(false);
		checkBoxAutoLogin->setChecked(false);
	}
	//新添加的头像显示功能
}

void LoginDialog::initSetting()
{
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	int x = (qApp->desktop()->width() - this->width()) / 2;
	int y = (qApp->desktop()->height() - this->height()) / 2;
	this->move(x, y);
}

void LoginDialog::on_pushButtonRegister_clicked()
{
	registerDlg = new RegisterDialog;
	registerDlg->show();
}

void LoginDialog::slotReadTcpDatagramFromServer()
{
	QDataStream in(tcpSocket);
	in >> loginResult >> userIdFromServer;
	if(loginResult == QString("CLIENT_LOGIN_SUCCESS"))
	{
		getUserInfoString();//这个函数是把账户输入的信息都转化为字符串,这样方便下一个方法写入xml;
		writePersonInfoIntoLocal();
		//弹出对话框
		friendListDlg = new FriendListDialog(p_dir->path(), userIdFromServer, passwdHash);
		friendListDlg->show();
		//tcpSocket->abort();
		tcpSocket->close();
		this->close();
	}
	else if(loginResult == QString("USER_NO_EXIST"))
	{
		QMessageBox::warning(0, QString::fromUtf8("错误!"), QString::fromUtf8("不存在此账户"), QMessageBox::Ok);
		lineEditPasswd->clear();
	}
	else if(loginResult == QString("CLIENT_LOGIN_FALES"))
	{
		QMessageBox::warning(0, QString::fromUtf8("错误!"), QString::fromUtf8("输入的密码有误"), QMessageBox::Ok);
		lineEditPasswd->clear();
	}
	else
	{
		QMessageBox::warning(0, QString::fromUtf8("错误!"), QString::fromUtf8("其他未知错误"), QMessageBox::Ok);
		lineEditPasswd->clear();
	}
}

void LoginDialog::on_checkBoxRemember_clicked(bool checked)
{
	rememberPasswd = checked;
}


void LoginDialog::on_pushButtonFind_clicked()
{
	findPasswdDlg = new FindPasswd;
	findPasswdDlg->setLoginName(userName_login);
	findPasswdDlg->show();
}

void LoginDialog::slotConnectToHostReady()
{
	pushButtonLogin->setEnabled(true);
}
