#include "FriendListDialog.h"
#include "FriendWidgetItem.h"
#include "FriendInfoDetailDialog.h"
#include <QListWidgetItem>
#include <QSqlQuery>
#include <QMouseEvent>
#include <QMenu>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QIODevice>
#include <QDesktopWidget>
#include <QSound>
#include "Cipher.h"
#include "LocalDataBase.h"
#include "TransferFileRemindForm.h"
#include "Struct/GroupInfoStruct.h"
#include "Struct/PersonalInfoStruct.h"
FriendListDialog::FriendListDialog(const QString &filePath, QString strUserID, QString strPasswd, QWidget *parent) :
	QDialog(parent), userID(strUserID), userPasswd(strPasswd)
{//尼玛忘记默认参数的要求了,默认参数右边是不能出现不能默认的参数的
	setupUi(this);
	initData(filePath);  //初始化一些数据
	initGui();				//初始化一些和界面相关的东西
	initSetting();			//用于设置无边框和显示的位置
	initNetWork();		//初始化网络部分
}

void FriendListDialog::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		offset = event->globalPos() - frameGeometry().topLeft();
	}
	else
	{
		QDialog::mousePressEvent(event);
	}
}

void FriendListDialog::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		move(event->globalPos() - offset);
	}
	else
	{
		QDialog::mouseMoveEvent(event);
	}
}

void FriendListDialog::slotItemdoublecheckd(QListWidgetItem *item)
{
	FriendWidgetItem *friendWidgetItem = dynamic_cast<FriendWidgetItem*>(item);

	SingleChatDialog *chatDlg = isChatDialogShow(friendWidgetItem->friendInfor.userID);
	if(chatDlg != NULL)
	{
		qDebug()<<"确实运行到这里了";
		chatDlg->raise();
		chatDlg->show();
		return;
	}
	else
	{
		chatDlg = new SingleChatDialog(db,userID.toInt(), friendWidgetItem->friendInfor.userID
									   ,friendWidgetItem->friendInfor.nickname ,personalInfo.nickname
									   ,friendWidgetItem->friendInfor.headImagePixmap);
		chatDlg->show();
		//关联聊天窗口和发信息的信号和好友列表处理的槽
		connect(chatDlg, SIGNAL(signalSendChatMessage(QByteArray&)), this, SLOT(slotRecChatMessage(QByteArray&)));
		connect(chatDlg, SIGNAL(signalTransferFile(QByteArray&)), this, SLOT(slotTransferFile(QByteArray&)));
		connect(chatDlg, SIGNAL(signalChatClose(const SingleChatDialog*)), this, SLOT(slotChatDlgClose(const SingleChatDialog*)));
		qDebug()<<friendWidgetItem->friendInfor.userID;
		hash_ID_chatDlg.insert(friendWidgetItem->friendInfor.userID, chatDlg);
	}
}

//好友的右键菜单
void FriendListDialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
	QAction *action=listMenu.exec(QCursor::pos());
	//在有动作的情况下
	if(action!=NULL)
	{
		FriendWidgetItem *fItem=dynamic_cast<FriendWidgetItem*>(item);	//大漠不要使用C风格的强转.......
		//修改备注的动作
		if(action->text()==tr("修改备注"))
		{
			qDebug()<<QString::fromUtf8("修改了好友的备注");
			bool OK=false;
			//弹出修改备注对话框
			QString newRemark;
			newRemark=QInputDialog::getText(
						this,
						tr("编辑备注"),
						tr("请输入新备注名"),
						QLineEdit::Normal,
						fItem->widgetNameMood.label->text(),
						&OK);
			if(OK)//若点击了OK(确定)
			{
				if(newRemark!="")
				{
					fItem->widgetNameMood.label->setText(newRemark);
				}
				else
				{
					fItem->widgetNameMood.label->setText(fItem->friendInfor.nickname);
				}
				fItem->friendInfor.mark=newRemark;
				//发送到服务端
				QByteArray byteData;
				QDataStream streamData(&byteData,QIODevice::WriteOnly);
				streamData.setVersion(QDataStream::Qt_4_7);
				streamData<<qint32(0)<<int(5)<<int(5)<<userID<<fItem->friendInfor.userID<<newRemark;
				streamData.device()->seek(0);
				streamData<<qint32(byteData.size()-(qint32)sizeof(qint32));
				//qDebug()<<"edit remark require size:"<<qint32(byteData.size()-(qint32)sizeof(qint32));
				tcpSocket->write(byteData);
				////////////////////////////////////////////////////////
				//不仅仅要更新到服务器并且要更新本地数据库
				QSqlQuery query(db);
				QString sqlUpdateCmd = QString("update friend_infor set friend_remarks = '%1' where friend_id = %2").arg(newRemark).arg(fItem->friendInfor.userID);
				//qDebug()<<"sqlUpdateCmd"<<endl<<sqlUpdateCmd<<endl;
				query.exec(sqlUpdateCmd);
			}
		}
		//删除好友的动作
		if(action->text()==tr("删除此人"))
		{
			int answer=QMessageBox::warning(
						this,
						tr("删除此人"),
						tr("要与之断绝关系么?"),
						QMessageBox::Yes|QMessageBox::No
						);
			if(answer==QMessageBox::Yes)
			{
				slotServer_deleteFriend(fItem->friendInfor.userID);
				//本地数据库也要delete
				localDatabase_deleteFriend(db,fItem->friendInfor.userID);
				listWidget_deleteFriend(fItem->friendInfor.userID);
			}
		}
		if(action->text()==tr("详细信息"))
		{
			slotShowPersonInforDlg(fItem->friendInfor,false);
		}
	}
}

void FriendListDialog::initGui()
{//都扔到构造函数那里太难看了所以把东西扔来这里
	QSqlQuery query(db);
	//query.setForwardOnly(true);
	QString strSelect = "select * from friend_infor";
	query.exec(strSelect);
	if(!query.isActive())
	{//用此函数来验证有没有错误发生,若无错则查询会变成"激活"状态
		//qDebug()<<query.lastError().text()<<endl;
		return;
	}
	PersonalInfoStruct tempInfo;
	while(query.next())
	{
		tempInfo.userID=query.value(0).toInt();
		tempInfo.mark=query.value(1).toString();
		tempInfo.nickname=query.value(2).toString();
		tempInfo.sex=query.value(3).toString();
		tempInfo.birthday=query.value(4).toDate();
		tempInfo.mail=query.value(5).toString();
		tempInfo.mood=query.value(6).toString();
		tempInfo.headImageByte=query.value(7).toByteArray();
		tempInfo.headImagePixmap.loadFromData(tempInfo.headImageByte);
		listWidget_insertFriend(tempInfo);
	}
	connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(slotItemdoublecheckd(QListWidgetItem*)));
	listWidget->setIconSize(QSize(48,48));
	//这个是用来读取并显示本地个人信息的
	localDatabase_readUserInfo(*userDir,personalInfo);
	refreshPersonalShowPart();
	//大漠定义了个系统菜单
	systemMenu.addAction(tr("修改个人信息"));
	systemMenu.addAction(tr("其它的请自行添加"));
	pushButtonSystem->setMenu(&systemMenu);
	connect(&systemMenu,SIGNAL(triggered(QAction*)),this,SLOT(on_systemMenu_itemClicked(QAction*)));
	//大漠在准备右键菜单
	listMenu.addAction(tr("修改备注"));
	listMenu.addAction(tr("删除此人"));
	listMenu.addAction(tr("详细信息"));
}

void FriendListDialog::initSetting()
{//本函数用于设置无边框和显示位置
	this->setWindowFlags(Qt::FramelessWindowHint);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	int x = qApp->desktop()->width() - this->width();
	int y = (qApp->desktop()->height() - this->height()) / 2;
	this->move(x, y);
}

void FriendListDialog::initNetWork()
{
	tcpSocket = new QTcpSocket(this);

	connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnectToServer()));//连接成功就发射信号
	connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slotReadTcpDatagramFromServer()));
	tcpSocket->connectToHost(QHostAddress("127.0.0.1"), 8888); //中心
}

void FriendListDialog::initData(const QString &filePath)
{
	messageCount = 0;
	messageRemindWidget = NULL;
	trayIcon = new QSystemTrayIcon(this);
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(slotIconActivated(QSystemTrayIcon::ActivationReason)));

	trayIcon->setVisible(true);
	trayIcon->hide();
	this->setTrayIcon(0);
	///////////////////////////////////////////
	//建立img目录
	userDir = new QDir(filePath);

	if(!userDir->exists("img"))
	{
		userDir->mkdir("img");
	}
	///////////////////////////////////////////////
	blockSize = 0;				//接收Tcp数据时用于计算大小的
	spinBoxIconSize->setMinimum(48);
	spinBoxIconSize->setMaximum(128);
	spinBoxIconSize->setValue(64);
	horizontalSliderIconSize->setMinimum(48);
	horizontalSliderIconSize->setMaximum(128);
	connect(spinBoxIconSize, SIGNAL(valueChanged(int)), horizontalSliderIconSize, SLOT(setValue(int)));
	connect(horizontalSliderIconSize, SIGNAL(valueChanged(int)), spinBoxIconSize, SLOT(setValue(int)));

	ConnectToLocalHostDatabase();		//打开本地的好友列表数据

	connect(imageLabel, SIGNAL(signalShowPersonInfoDlg()), this, SLOT(on_imageLabel_clicked()));
	connect(imageLabel, SIGNAL(signalUpdataPersonInforToServer()), this, SLOT(slotUpdatePersonInfor()));
	connect(moodLineEdit, SIGNAL(signalMoodString(QString)), this, SLOT(slotMoodString(QString)));

	connect(pushButtonClose, SIGNAL(clicked()), qApp, SLOT(quit()));		//
}

void FriendListDialog::ConnectToLocalHostDatabase()
{
	db = QSqlDatabase::addDatabase("QSQLITE");
	QString databaseName = "/chat.db";
	QString databasePath = userDir->path() + databaseName;
	////////////////////////////////
	//qDebug()<<"databasePath"<<databasePath<<endl;
	//////////////////////////////////
	db.setDatabaseName(databasePath);

	if(db.open())
	{
		///打开数据库就创建表
		QSqlDatabase query(db);
		query.exec("create table friend_infor ("
				   "friend_id INT PRIMARY KEY,"
				   "friend_remarks VARCHAR(10),"
				   "friend_nickname VARCHAR(10) NOT NULL,"
				   "friend_sex CHAR(1) DEFAULT 'M',"
				   "friend_birthday DATE DEFAULT '1990-1-1',"
				   "friend_mail VARCHAR(30),"
				   "friend_mood VARCHAR(100),"
				   "friend_headimage MEDIUMBLOB)");
	}
	else
	{
		qDebug()<<tr("FriendListDialog.cpp--- chat data can't open!!!!!")<<endl;
	}
}

void FriendListDialog::tcpDatagramToLocalHostDatabase(QDataStream &in)
{
	QSqlQuery query(db);
	//先清空本地旧数据和旧好友列表
	if(query.exec("delete from friend_infor"))
	{//在这个地方删除的代码千万不能写成 delete * from friend_infor 这样的话删除不了的
		qDebug()<<"delete successfully";
	}
	else
	{
		qDebug()<<"delete failed!!!";
	}
	listWidget->clear();
	//然后写新数据入
	PersonalInfoStruct tempInfo;
	while (!in.atEnd())
	{
		in
				>>tempInfo.userID
			   >>tempInfo.nickname
			  >>tempInfo.sex
			 >>tempInfo.birthday
			>>tempInfo.mail
		   >>tempInfo.mood
		  >>tempInfo.headImageByte
		 >>tempInfo.mark;
		tempInfo.headImagePixmap.loadFromData(tempInfo.headImageByte);
		///验证数据接收成功,现在要把这些数据写进本地的SQLite数据库中
		query.prepare("insert into friend_infor values(?, ?, ?, ?, ?, ?, ?, ?)");
		query.addBindValue(tempInfo.userID);
		query.addBindValue(tempInfo.mark);
		query.addBindValue(tempInfo.nickname);
		query.addBindValue(tempInfo.sex);
		query.addBindValue(tempInfo.birthday);
		query.addBindValue(tempInfo.mail);
		query.addBindValue(tempInfo.mood);
		query.addBindValue(tempInfo.headImageByte);
		query.exec();
		//然后更新好友列表
		qDebug()<<QString::fromUtf8("更新好友列表");
		listWidget_insertFriend(tempInfo);//因为之前清空了表,故insert
		localDatabase_insertFriend(db,tempInfo);//同理,数据库清空了,故也insert
		//判断并新建聊天记录表
		QString tablename = "chatlog" + QString::number(tempInfo.userID);
		qDebug()  << "chatlog" << tablename;
		QString sqltemp  = QString("create table if not exists %1 ("
								   "chatlog_id integer primary key,"
								   "chat_date datetime,"
								   "chat_text_html mediumblob,"
								   "chat_text mediumblob,"
								   "is_send char(1))").arg(tablename);
		//chatlog_id 不填写，让其自动增长，is_send为0表示接收到的信息，为1表发送
		query.exec(sqltemp);
	}
	slotServer_getUserInfo();//构造完了,最后才拿自己的信息
}


void FriendListDialog::slotConnectToServer()
{
	QByteArray byteData;
	byteData.clear();
	QDataStream out(&byteData, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << (qint32)0;//站位用
	out << (qint32)4;//先请求好友个人信息(大漠留言:先人后己)
	out << (qint32)userID.toInt();//源

	//	qDebug()<<"FriendListDialog.cpp--------- userID"<<userID<<endl;
	out.device()->seek(0);
	out << (qint32)(byteData.size() - (qint32)sizeof(qint32));
	//	qDebug()<<byteData.size()-(qint32)sizeof(qint32);
	tcpSocket->write(byteData);
}

void FriendListDialog::slotReadTcpDatagramFromServer()
{
	QDataStream in(tcpSocket);
	in.setVersion(QDataStream::Qt_4_7);

	if(0 == blockSize)
	{
		if(tcpSocket->bytesAvailable() < (qint32)sizeof(qint32))
		{
			return;
		}
		in >> blockSize;
	}

	if(tcpSocket->bytesAvailable() < blockSize)
	{
		return;
	}
	/*
	if(tcpSocket->bytesAvailable() == blockSize)
	{
		in>> id_des >> id_src;
	}
*/
	in>> id_des >> id_src;
	//	qDebug()<< destinationID <<"heather";
	switch (id_des)
	{
		case 1:
		{
			//qDebug()<<QString::fromUtf8("客户端接收到服务器发来的初始化好友列表的的数据报");
			tcpDatagramToLocalHostDatabase(in);
			break;
		}
		case 2:
		{
			//qDebug()<<QString::fromUtf8("客户端接收到服务器发来的初始自己的个人信息的数据报");
			slotClient_getUserInfo(in);
			break;
		}
		case 3:
		{
			//		qDebug()<<"got update friend info!";
			slotClient_updateFriend(in);
			break;
		}
		case 7:
		{
			//		qDebug()<<"friend infor";
			seekOther->readFriendInformationFromServer(in);
			break;
		}
		case 8:
		{
			qDebug()<<"add friend request";
			in>>transmitAddFriendId>>transmitAddSourceId>>transmitAddAccount>>transmitAddNickname>>transmitAddTime;
			qDebug()<<QString("转发到有好友添加请求")<<transmitAddFriendId<<transmitAddSourceId<<transmitAddAccount<<transmitAddNickname<<transmitAddTime;
			QPixmap pix(":/more.png");
			QString str = QString("添加好友请求");
			QString htmlStr = "8";//这里的12用来表示消息类型是添加好友请求。
			showShortMessage(transmitAddSourceId, pix,  str,  htmlStr);
			break;
		}
		case 9:
		{//接收好友添加成功通知。
			qDebug()<<"agree add Friend infor";
			in>>requesterId>>friendId>>time;
			QPixmap pix(":/Logo.png");
			QString str = QString("添加好友成功");
			QString htmlStr = "9";//这里的12用来表示消息类型是添加好友请求。
			showShortMessage(friendId, pix,  str,  htmlStr);
			break;
		}
		case 10:
		{
			//			qDebug()<<"have add friend, can't add again";
			QMessageBox::about(0, QString::fromUtf8("信息"),QString::fromUtf8("对方已经是你的好友，不可再添加"));
			break;
		}
		case 11:
		{
			//		qDebug()<<"waiting for friend agree";
			QMessageBox::about(0, QString::fromUtf8("添加好友"), QString::fromUtf8("添加好友成功，正在等待对方回复... ..."));
			break;
		}
		case 12:
		{
			qDebug()<<"有人要发文件给你";
			getTransferFileInfor(in, id_src);
			break;
		}
		case 13:
		{
			qDebug()<<"文件的接收方不在线";
			transFileDesOffline(id_src);
			break;
		}
		case 40:
		{

			//			chatDlg->readMessage(in);
			break;
		}
		default:
		{
			if(id_des >= USER_ID_START)
			{
				qDebug()<<blockSize;
				qDebug()<<"第一次未读消息之前剩余的数据包大小"<<tcpSocket->bytesAvailable();
				getChatMessage(in);
				qDebug()<<blockSize;
				qDebug()<<"第一次未读消息之后剩余的数据包大小"<<tcpSocket->bytesAvailable();
			}
			else
			{
				qDebug()<< QString::fromUtf8("未知消息!")<<endl;
			}
			break;
		}
	}
	blockSize=0;
}

void FriendListDialog::on_systemMenu_itemClicked(QAction *action)
{
	if(action->text()==tr("修改个人信息"))
	{
		slotShowPersonInforDlg(personalInfo,true);
	}
}

//这个好友列表操作的常用函数
FriendWidgetItem* FriendListDialog::listWidget_searchFriend(qint32 id)
{
	FriendWidgetItem* item;
		int count=listWidget->count();
		for(int i=0;i<count;++i)
		{
			item= dynamic_cast<FriendWidgetItem*>(listWidget->item(i));
			if(item->friendInfor.userID==id)
			{
				return item;
			}
		}
		return NULL;
}
bool FriendListDialog::listWidget_insertFriend(PersonalInfoStruct& friendInfo)
{
	qDebug()<<"listWidget_insertFriend()";
	FriendWidgetItem* item=listWidget_searchFriend(friendInfo.userID);
	if(item!=NULL)
	{
		qDebug()<<"好友列表——插入失败";
		return false;
	}
	//先创建一个ITEM
	FriendWidgetItem *fItem=new FriendWidgetItem(friendInfo,*completeLineEdit);
	//nice我在这里就可以关联id和头像了
	hash_ID_headImage.insert(friendInfo.userID,fItem->friendInfor.headImagePixmap);
	hash_ID_NickName.insert(friendInfo.userID,fItem->friendInfor.nickname);
	listWidget->addItem(fItem);
	listWidget->setItemWidget(fItem,fItem->widgetNameMood.topLevelWidget());
	return true;
}
bool FriendListDialog::listWidget_updateFriend(PersonalInfoStruct& friendInfo)
{
	qDebug()<<"listWidget_updateFriend()";
	FriendWidgetItem* item=listWidget_searchFriend(friendInfo.userID);
	if(item==NULL)
	{
		qDebug()<<"好友列表——更新失败";
		return false;
	}
	item->friendInfor=friendInfo;
	hash_ID_headImage.insert(friendInfo.userID,item->friendInfor.headImagePixmap);
	hash_ID_NickName.insert(friendInfo.userID,item->friendInfor.nickname);
	//更新视图部分
	item->setIcon(item->friendInfor.headImagePixmap);
	item->widgetNameMood.strName=item->friendInfor.nickname;
	item->widgetNameMood.strMark=item->friendInfor.mark;
	item->widgetNameMood.strMood=item->friendInfor.mood;
	item->widgetNameMood.refresh();
	return true;
}
bool FriendListDialog::listWidget_deleteFriend(qint32 id)
{
	qDebug()<<"listWidget_deleteFriend()";
	FriendWidgetItem* item=listWidget_searchFriend(id);
	if(item==NULL)
	{
		qDebug()<<"好友列表——删除失败";
		return false;
	}
	delete listWidget->takeItem(listWidget->row(item));
	hash_ID_NickName.remove(id);
	hash_ID_headImage.remove(id);
	return true;
}

void FriendListDialog::on_spinBoxIconSize_valueChanged(int arg1)
{
	listWidget->setIconSize(QSize(arg1,arg1));
}
void FriendListDialog::on_pushButtonFindOther_clicked()
{
	seekOther = new SeekClientDialog;
	seekOther->getTcpSocket(tcpSocket);
	seekOther->setSourseId(userID.toInt());
	seekOther->show();
	pushButtonFindOther->setEnabled(false);
	connect(seekOther, SIGNAL(signalSeekDlgClose()), this, SLOT(slotSetSeekPushEnable()));
}

//大漠写的,显示个人信息的槽,用到了HASH表
void FriendListDialog::slotShowPersonInforDlg(PersonalInfoStruct &info,bool allowEdit)
{
	//先检测是否已经显示了此窗体
	if(hash_ID_InfoDetailDlg.count(info.userID)==0)//注意:返回值为0是未找到,1是找到1条,2是找到2条,以此类推
	{
		qDebug()<<"显示"<<info.userID<<"的详细信息";
		friendInfoDlg = new FriendInfoDetailDialog(info,allowEdit);	//初始化一个个人的详细信息
		friendInfoDlg->show();
		if(allowEdit)
		{
			connect(friendInfoDlg,SIGNAL(signalGetMyInfo()),this,SLOT(slotServer_getUserInfo()));
			connect(friendInfoDlg,SIGNAL(signalSetMyInfo(PersonalInfoStruct&)),this,SLOT(slotServer_setUserInfo(PersonalInfoStruct&)));
		}
		else
		{
			connect(friendInfoDlg,SIGNAL(signalUpdateFriendInfo(qint32)),this,SLOT(slotServer_updateFriend(qint32)));
		}
		connect(friendInfoDlg,SIGNAL(signalClose(qint32)),this,SLOT(slotFriendInfoDlgClose(qint32)));
		//扔进表里
		hash_ID_InfoDetailDlg.insert(info.userID,friendInfoDlg);
	}
	else
	{
		qDebug()<<info.userID<<"的详细信息窗体已经加载";
		hash_ID_InfoDetailDlg.value(info.userID)->raise();
	}
}

void FriendListDialog::slotUpdatePersonInfor()
{
	if(imageLabel->getImageDataByte()!=personalInfo.headImageByte)
	{
		//PersonalInfoStruct newPersonalInfo = personalInfo;
		personalInfo.headImageByte=imageLabel->getImageDataByte();
		slotServer_setUserInfo(personalInfo);
	}
}

void FriendListDialog::slotMoodString(const QString &str)
{//用于接收改变心情时候发出的信号,该信号把心情作为参数传递
	if(personalInfo.mood != str)
	{
		//PersonalInfoStruct newPersonalInfo = personalInfo;
		personalInfo.mood=str;
		slotServer_setUserInfo(personalInfo);
	}
}

void FriendListDialog::slotRecChatMessage(QByteArray &chatDataByte)
{//收到聊天窗口发来的信号,就把聊天信息发给服务器
	qDebug()<<"发送聊天数据之前数据大小为"<<chatDataByte.size();
	tcpSocket->write(chatDataByte);
}

void FriendListDialog::slotIconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
		case QSystemTrayIcon::Trigger:
		case QSystemTrayIcon::DoubleClick:
		case QSystemTrayIcon::MiddleClick:
		{
			if(messageCount > 0)
			{//有未读消息
				//
			}
			else if(messageCount == 0)
			{
				qDebug()<<"现在应该弹出的是好友列表界面";
				trayIcon->hide();
				this->raise();	//添加这一句使得窗口显示的时候在Z轴最顶层
				this->show();
			}
			break;
		}
		default:
		{
			qDebug()<<"我都不知道还能按下什么案件了";
		}
	}
}

void FriendListDialog::slotShowChatMessage(QModelIndex index)
{//在这里调出聊天窗口
	qDebug()<<"调出聊天的窗口~~把聊天信息显示出来";
	//QString nickName_friend = index.data().toString();
	QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
	qint32 id = index.data(Qt::UserRole + 1).value<int>();
	QString chatHtml = index.data(Qt::UserRole + 2).toString();

	QPixmap img_friend = icon.pixmap(QSize(128,128));
	if(img_friend.isNull())
	{
		qDebug()<<"Qt::DecorationRole出来的头像又是空的了";
	}

	SingleChatDialog *chatDlg = isChatDialogShow(id);
	if(NULL == chatDlg)
	{
		qDebug()<<"这个聊天窗口之前没有打开过";
		//既然这样就得生成一个新的聊天窗口
		chatDlg = new SingleChatDialog(db,id_des, id_src, hash_ID_NickName.value(id) , personalInfo.nickname, img_friend);
		connect(chatDlg, SIGNAL(signalSendChatMessage(QByteArray&)), this, SLOT(slotRecChatMessage(QByteArray&)));
		connect(chatDlg, SIGNAL(signalTransferFile(QByteArray&)), this, SLOT(slotTransferFile(QByteArray&)));
		connect(chatDlg, SIGNAL(signalChatClose(const SingleChatDialog*)), this, SLOT(slotChatDlgClose(const SingleChatDialog*)));
		hash_ID_chatDlg.insert(id, chatDlg);
	}
	//在这里插入信息
	chatDlg->insertNickName_DateTime(hash_ID_NickName.value(id), timeMessageSend, 1);//这里最后一个参数1代表是别人发来的信息
	//qDebug()<<chatHtml;
	chatDlg->inserChatMessage(chatHtml);
	qDebug()<<hash_ID_NickName;
	qDebug()<< id<<"111111111111111111111111111111111111111111111111";
	chatDlg->raise();
	chatDlg->show();
}

void FriendListDialog::slotChatDlgClose(const SingleChatDialog *p_chat)
{
	SingleChatDialog *p = const_cast<SingleChatDialog *>(p_chat);
	p->disconnect();
	QHash<int, SingleChatDialog*>::iterator i;
	for(i = hash_ID_chatDlg.begin(); i != hash_ID_chatDlg.end(); ++i)
	{
		if(p_chat == i.value())
		{//如果找到已经关闭的窗口的地址,就在这里删除
			int id = i.key();
			hash_ID_chatDlg.remove(id);
			return;
		}
	}
}

void FriendListDialog::slotShortMessageClose(const MessageRemindForm *p)
{
	qDebug()<<"他关闭了";
	MessageRemindForm *p1 = const_cast<MessageRemindForm *>(p);
	p1->disconnect();	//竟然是这样的......
	messageRemindWidget = NULL;
}

//大漠用来刷新的
void FriendListDialog::refreshPersonalShowPart()
{
	labelNickName->setText(personalInfo.nickname);
	moodLineEdit->setMoodString(personalInfo.mood);
	imageLabel->loadImage(personalInfo.headImageByte);
}

void FriendListDialog::getChatMessage(QDataStream &in)
{
	qDebug()<<"有新的消息到来";
	++messageCount;	//有信息来
	QByteArray imgDataByte;
	qint32 imgCount;
	QString chat;//聊天内容的存文字版 html则是聊天内容的html版
	QString chatHtml;
readthis:	in >> imgCount >> imgDataByte >> chatHtml >> chat >> timeMessageSend;
	QDataStream imgIn(&imgDataByte, QIODevice::ReadOnly);

	for(int i = 0; i < imgCount; ++i)
	{
		QByteArray temp;
		imgIn >> temp;
		QPixmap pix;
		pix.loadFromData(temp, "PNG");
		QString imagePath = QString("temp") + "/" + QString::number(i+1) + ".png";
		pix.save(imagePath, "PNG");
	}
	//恩.这里缺少一个消息提醒这样的功能,来信息了之后要知道有信息来了
	//QApplication::beep(); //你奶
	//QSound::play("Windows Shutdown.wav");
	//好吧我还是写一个提醒框比较好
	QPixmap pix = hash_ID_headImage.value(id_src);
	if(pix.isNull())
	{
		qDebug()<<"hash出来就没有值";
		//如果以后开放陌生人这样的临时会话的话就在这里加载一个默认的 头像
		pix = QPixmap(":/Logo.png");
	}
	////////////////////////////////////////////
	//添加如果已经打开该窗口的判断,下面的判断部分应该写成一个函数滴...
	SingleChatDialog *chatDlg = isChatDialogShow(id_src);
	if(NULL != chatDlg)
	{//////////////////////////////////////////
		chatDlg->insertNickName_DateTime(hash_ID_NickName.value(id_src), timeMessageSend, 1);//这里最后一个参数1代表是别人发来的信息
		//qDebug()<<html2;
		chatDlg->inserChatMessage(chatHtml);
	}
	else
	{
		showShortMessage(id_src, pix, chat, chatHtml);
		//showShortMessage(id_des, pix, chat, chatHtml);
	}
	//////////////////////////////////////////////////
	////////////出错拉～～～出错原因是因为表！！！表名！！！要传ID参数过去 明天继续

	chatDlg->writeChatLogDb(db,id_src,timeMessageSend,chat,chatHtml,0);//插入聊天记录数据库，参数为为0表示接收到的信息

	//query.addBindValue(textEditChat->toPlainText());
	// query.addBindValue(textEditChat->toHtml());
	if(!in.atEnd())
	{
		qint32 temp;
		in >> temp;
		in >> id_des;
		in >> id_src;
		qDebug()<< "看看读出什么"<<temp<<id_des<<id_src;
		goto readthis;
	}
}

void FriendListDialog::setTrayIcon(int type)
{
	if(MESSAGE_NO_COME == type)
	{
		QIcon icon(":/Logo.png");
		trayIcon->setIcon(icon);
		this->setWindowIcon(icon);
	}
	else if(MESSAGE_COME == type)
	{
		trayIcon->setIcon(QIcon(":/messages_come.png"));
		this->setWindowIcon(QIcon(":/messages_come.png"));
		this->setWindowIcon(QIcon(":/messages_come.png"));
	}
	else
	{
		qDebug()<<"错误的图标显示类型";
	}
}

void FriendListDialog::showShortMessage(qint32 id, QPixmap headimage, QString str, QString chatHtml)
{
	qDebug()<<"看看showShortMessage()这里要被调用多少次";

	if(NULL == messageRemindWidget)
	{
		messageRemindWidget = new MessageRemindForm;
		if("8" == chatHtml)
		{
			connect(messageRemindWidget, SIGNAL(signalClickedItem(QModelIndex)), this, SLOT(slotAddFfiendDlg(QModelIndex)));
		}
		else if("9" == chatHtml)
		{
			connect(messageRemindWidget, SIGNAL(signalClickedItem(QModelIndex)), this, SLOT(slotAgreeAddFriendDla(QModelIndex)));
		}
		else
		{
			connect(messageRemindWidget, SIGNAL(signalClickedItem(QModelIndex)), this, SLOT(slotShowChatMessage(QModelIndex)));
		}
		connect(messageRemindWidget, SIGNAL(signalMessageReminFormClose(const MessageRemindForm*)),
				this, SLOT(slotShortMessageClose(const MessageRemindForm*)));
	}
	if(headimage.isNull())
	{
		qDebug()<<"要弹出消息框时头像为空";
	}
	messageRemindWidget->show();
	messageRemindWidget->addMessage(id, headimage, str, chatHtml);
}

void FriendListDialog::closeEvent(QCloseEvent *event)
{
	qDebug()<<"按下关闭拉";
	if (trayIcon->isVisible())
	{
		this->hide();
		event->ignore();
	}
	/*
	else
	{
		//QDialog::closeEvent(event);
	}
	*/
}

void FriendListDialog::on_pushButtonMinSize_clicked()
{
	trayIcon->show();
	this->close();
}

SingleChatDialog* FriendListDialog::isChatDialogShow(qint32 id)
{
	SingleChatDialog *chatDlg = NULL;
	QHash<qint32, SingleChatDialog*>::const_iterator i;
	for(i = hash_ID_chatDlg.constBegin(); i != hash_ID_chatDlg.constEnd(); ++i)
	{
		//qDebug()<<"id_des"<<id_des<<"  "<<"i.key"<<i.key();
		if(id == i.key())
		{//如果存在重复的对话窗口就不要在继续打开
			chatDlg = i.value();
			break;
		}
	}
	return chatDlg;
}

void FriendListDialog::getMessage()
{
	QByteArray byte;
	QDataStream out(&byte, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);

	out << (qint32)0;
	out << (qint32)32;
	out << (qint32)userID.toInt();
	out.device()->seek(0);
	out << (qint32)(byte.size() - sizeof(qint32));
	tcpSocket->write(byte);
}

void FriendListDialog::slotAddFfiendDlg(QModelIndex index)
{
	acceptAddFriendRequet = new AcceptAddFriendRequet;
	acceptAddFriendRequet->getTcpSocket(tcpSocket);
	acceptAddFriendRequet->transmitAddFriendSetGui(transmitAddSourceId, transmitAddAccount, transmitAddNickname, transmitAddTime);
	acceptAddFriendRequet->setTwoUserID(transmitAddFriendId, transmitAddSourceId);
	acceptAddFriendRequet->show();
}

void FriendListDialog::slotAgreeAddFriendDla(QModelIndex index)
{
	acceptServerInfor = new AcceptServerInfor;
	acceptServerInfor->readNotifyAgreeAddFriend(requesterId, friendId, time);
	acceptServerInfor->show();
}

void FriendListDialog::slotSetSeekPushEnable()
{
	pushButtonFindOther->setEnabled(true);
}

void FriendListDialog::slotFriendInfoDlgClose(qint32 id)//这是关闭详细信息表时需要做的事
{
	friendInfoDlg=hash_ID_InfoDetailDlg.value(id);
	disconnect(friendInfoDlg,SIGNAL(signalClose(qint32)),this,SLOT(slotFriendInfoDlgClose(qint32)));
	hash_ID_InfoDetailDlg.remove(id);
}

void FriendListDialog::slotTransferFile(QByteArray &chatDataByte)
{
	tcpSocket->write(chatDataByte);
}

void FriendListDialog::getTransferFileInfor(QDataStream &in, qint32 id)
{//获取别人要发来的文件的基本信息如大小,文件名
	in >> fileSize >> fileName >> iconByte;
	qDebug()<<"你的好友:"<< hash_ID_NickName.value(id)<<"要给你发送文件";
	//qDebug()<<"将要接收的文件的大小是"<<fileSize<<"文件名是:"<<fileName;

	SingleChatDialog *chatDlg = isChatDialogShow(id_src);
	if(NULL != chatDlg)
	{//聊天窗口是已经打开过了的,那我就应该直接在聊天窗口中给出提示
		//在这里提示用户处理 接收文件的请求

	}
	else
	{//聊天窗口还没有打开,那就用另外一个东西来提示用户
		transferFileRemindForm =new TransferFileRemindForm(hash_ID_headImage.value(id), iconByte, hash_ID_NickName.value(id),
														   id, fileSize, fileName);
		transferFileRemindForm->show();
	}
}

void FriendListDialog::transFileDesOffline(qint32 id_friend)
{
	QString str =QString("    因为您的好友没有在线,所以文件不能成功发送给 ") + hash_ID_NickName.value(id_friend) +
			" 现在你要把文件保存到离线服务器么? 选择OK将把文件保存到离线服务器,这样当您的好友下次登录的时候就能接受到文件传输的请求. 选择Cancel取消本次文件发送";
	QMessageBox::information(0, "文件发送请求失败", str, QMessageBox::Ok, QMessageBox::Cancel);
}

void FriendListDialog::on_imageLabel_clicked()
{
	slotShowPersonInforDlg(personalInfo,true);
}
//下面这堆是大漠写的请求类型
void FriendListDialog::slotServer_getUserInfo()
{
	qDebug()<<"slotServer_getUserInfo()";
	QByteArray bytes;
	QDataStream streamData(&bytes,QIODevice::WriteOnly);
	streamData<<(qint32)0;
	streamData<<(qint32)2<<(qint32)userID.toInt();//2是自己的信息
	streamData.device()->seek(0);
	streamData<<(qint32)(bytes.size()-sizeof(qint32));
	tcpSocket->write(bytes);
}
void FriendListDialog::slotServer_setUserInfo(PersonalInfoStruct &newInfo)
{
	qDebug()<<"slotServer_setUserInfo()";
	//更新需要显示的部分
	refreshPersonalShowPart();//先刷新后改写是为了防止出错,估计是QT类本身的问题
	personalInfo=newInfo;
	//发送到服务器
	QByteArray bytes;
	QDataStream streamData(&bytes,QIODevice::WriteOnly);
	streamData<<(qint32)0<<(qint32)30<<newInfo.userID;
	streamData<<newInfo.nickname
			 <<newInfo.sex
			<<newInfo.birthday.toString("yyyy-MM-dd")
		   <<newInfo.mail
		  <<newInfo.mood
		 <<newInfo.headImageByte;
	streamData.device()->seek(0);
	streamData<<(qint32)(bytes.size()-sizeof(qint32));
	tcpSocket->write(bytes);
	//写回本地文件
	refreshPersonalShowPart();
	localDatabase_writeUserInfo(*userDir,personalInfo);
}
void FriendListDialog::slotServer_insertFriend(qint32 friendID)
{
	qDebug()<<"slotServer_insertFriend()";
	//添加好友的请求
}
void FriendListDialog::slotServer_updateFriend(qint32 friendID)
{
	qDebug()<<"slotServer_updateFriend()";
	//更新好友的请求
	QByteArray byteData;
	QDataStream streamData(&byteData,QIODevice::WriteOnly);
	streamData<<(qint32)0<<(qint32)31<<personalInfo.userID<<friendID;
	//发送
	streamData.device()->seek(0);
	streamData<<(qint32)(byteData.size()-sizeof(qint32));
	tcpSocket->write(byteData);
}
void FriendListDialog::slotServer_deleteFriend(qint32 friendID)
{
	qDebug()<<"slotServer_deleteFriend()";
	//删除好友的请求
	QByteArray byteData;
	QDataStream streamData(&byteData,QIODevice::WriteOnly);
	streamData.setVersion(QDataStream::Qt_4_7);
	streamData<<qint32(0)<<int(6)<<int(6)<<userID<<friendID;
	streamData.device()->seek(0);
	streamData<<qint32(byteData.size()-(qint32)sizeof(qint32));
	tcpSocket->write(byteData);
}
//下面这堆是大漠写的相应请求的类型
void FriendListDialog::slotClient_getUserInfo(QDataStream &in)
{
	qDebug()<<"slotClient_getUserInfo()";
	//开始读取个人信息
	PersonalInfoStruct tempInfo;
	in>>tempInfo.userID
	>>tempInfo.nickname
	>>tempInfo.sex
	>>tempInfo.birthday
	>>tempInfo.mail
	>>tempInfo.mood
	>>tempInfo.headImageByte;
	tempInfo.headImagePixmap.loadFromData(tempInfo.headImageByte);
	tempInfo.mark="";//自己没必要给自己写备注
	//设置存储部分
	personalInfo=tempInfo;
	//设置显示部分,并且写回本地文件
	//	qDebug()<<"personalInfo: "<<friend_id<<friend_nickName<<friend_sex<<friend_birthday.toString()<<friend_mail<<friend_mood;
	refreshPersonalShowPart();
	localDatabase_writeUserInfo(*userDir,personalInfo);
	getMessage();
	//若显示了此窗体,则添加
	if(hash_ID_InfoDetailDlg.count(tempInfo.userID))
	{
		hash_ID_InfoDetailDlg.value(tempInfo.userID)->updateGui(tempInfo);
	}
}
void FriendListDialog::slotClient_setUserInfo(QDataStream &in)
{
	qDebug()<<"slotClient_setUserInfo()";
}
void FriendListDialog::slotClient_insertFriend(QDataStream &in)
{
	qDebug()<<"slotClient_insertFriend()";
	//收到了对方同意添加的消息
}
void FriendListDialog::slotClient_updateFriend(QDataStream &in)
{
	qDebug()<<"slotClient_updateFriend()";
	PersonalInfoStruct tempInfo;
	in>>tempInfo.userID
	>>tempInfo.nickname
	>>tempInfo.sex
	>>tempInfo.birthday
	>>tempInfo.mail
	>>tempInfo.mood
	>>tempInfo.headImageByte
	>>tempInfo.mark;
	tempInfo.headImagePixmap.loadFromData(tempInfo.headImageByte);
	//读完后直接更新列表
	listWidget_updateFriend(tempInfo);
	localDatabase_updateFriend(db,tempInfo);
	//若打开了显示详细信息的窗体,则更新
	if(hash_ID_InfoDetailDlg.count(tempInfo.userID))
	{
		hash_ID_InfoDetailDlg.value(tempInfo.userID)->updateGui(tempInfo);
	}
}
void FriendListDialog::slotClient_deleteFriend(QDataStream &in)
{
	qDebug()<<"slotClient_deleteFriend()";
	//收到了服务端请求发回来的消息
}