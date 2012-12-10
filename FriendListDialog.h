#ifndef FRIENDLISTDIALOG_H
#define FRIENDLISTDIALOG_H

#include "ui_FriendListDialog.h"
#include "Struct/PersonalInfoStruct.h"
#include "FriendInfoDetailDialog.h"
#include "FriendWidgetItem.h"
#include "Widget_Name_Mood.h"
#include <QDir>
#include <QSqlDatabase>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QInputDialog>
#include <QTcpSocket>
#include <QSystemTrayIcon>
#include"seekclientdialog.h"
#include"acceptaddfriendrequet.h"
#include"acceptserverinfor.h"
#include "SingleChatDialog.h"
#include "MessageRemindForm.h"
#include "TransferFileRemindForm.h"
#include "Struct/GroupInfoStruct.h"

class FriendWidgetItem;
class FriendInfoDetailDialog;

const int USER_ID_START = 100000000;
const int MESSAGE_NO_COME = 0;
const int MESSAGE_COME = 1;

class FriendListDialog : public QDialog, private Ui::FriendListDialog
{
	Q_OBJECT
public:
	explicit FriendListDialog(const QString &filePath, QString strUserID, QString strPasswd, QWidget *parent = 0);
signals:
	void signalFriendInfoIsUpdated(FriendWidgetItem *friendItem);
protected:
	void closeEvent(QCloseEvent *event);			//在这里重写关闭事件实现最小化
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
private slots:

	void on_listWidget_itemClicked(QListWidgetItem *item);//列表里好友的右击事件
	void on_systemMenu_itemClicked(QAction *action);//这是大漠添加的系统菜单单击事件
	void on_spinBoxIconSize_valueChanged(int arg1);
	void on_pushButtonFindOther_clicked();
	void on_pushButtonMinSize_clicked();
	void on_imageLabel_clicked();//后期添加功能时才发现不合理,这个主要是修正部长的派生imageLabel的问题

	void slotItemdoublecheckd(QListWidgetItem *item);
	void slotConnectToServer();
	void slotReadTcpDatagramFromServer();
	void slotShowPersonInforDlg(PersonalInfoStruct &info,bool allowEdit);		//用于当点击个人的头像的时候弹出大漠写的个人信息的窗口
	void slotUpdatePersonInfor();
	void slotMoodString(const QString &str);	//用于接收改变心情时候发出的信号,该信号把心情作为参数传递
	void slotRecChatMessage(QByteArray &chatDataByte);				//有聊天信息来
	void slotIconActivated(QSystemTrayIcon::ActivationReason reason);	//左键点击通知栏图标
	void slotShowChatMessage(QModelIndex index);		//在这里调出聊天窗口
	void slotChatDlgClose(const SingleChatDialog *p_chat);
	void slotShortMessageClose(const MessageRemindForm *p);
	void slotAddFfiendDlg(QModelIndex index);//在这里调出添加友好请求窗口。
	void slotAgreeAddFriendDla(QModelIndex index);
	void slotSetSeekPushEnable();
	void slotFriendInfoDlgClose(qint32 id);
	void slotTransferFile(QByteArray &chatDataByte);
	//这些是发消息给服务器的槽(作者:大漠)
	void slotServer_getUserInfo();//获取个人信息
	void slotServer_setUserInfo(PersonalInfoStruct &newInfo);//写回个人信息
	void slotServer_insertFriend(qint32 friendID);//请求添加好友
	void slotServer_updateFriend(qint32 friendID);//请求更新好友
	void slotServer_deleteFriend(qint32 friendID);//请求删除好友
	//这些是处理服务器发回来的消息(作者:大漠)
	void slotClient_getUserInfo(QDataStream &in);
	void slotClient_setUserInfo(QDataStream &in);
	void slotClient_insertFriend(QDataStream &in);
	void slotClient_updateFriend(QDataStream &in);
	void slotClient_deleteFriend(QDataStream &in);

private:
	//这些是好友列表的操作
	FriendWidgetItem* listWidget_searchFriend(qint32 id);
	bool listWidget_insertFriend(PersonalInfoStruct& friendInfo);
	bool listWidget_updateFriend(PersonalInfoStruct& friendInfo);
	bool listWidget_deleteFriend(qint32 id);
	//这些是初始化的
	void initGui();
	void initSetting();//本函数用于设置无边框还有显示的位置
	void initNetWork();
	void initData(const QString &filePath);
	//下面这些杂七杂八
	void ConnectToLocalHostDatabase();
	void tcpDatagramToLocalHostDatabase(QDataStream &in);
	void refreshPersonalShowPart();//这是用来刷新显示部分的,也就是个人的昵称,心情,头像
	void getChatMessage(QDataStream &in);
	void setTrayIcon(int type);		//用于更改图标的
	void showShortMessage(qint32 id, QPixmap headimage, QString str, QString chatHtml);
	SingleChatDialog* isChatDialogShow(qint32 id);
	void getMessage();  //获取离线的消息
	void getTransferFileInfor(QDataStream &in, qint32 id);//获取别人要发来的文件的基本信息如大小,文件名
	void transFileDesOffline(qint32 id_friend);
private:
	QPoint offset;
	QSqlDatabase db;

	QMenu listMenu;//好友列表的右键菜单
	QMenu systemMenu;//系统菜单,常用系统操作

	QString userID;
	QString userPasswd;	//这个是经过MD5加密之后的密码

	QDir *userDir;

	QTcpSocket *tcpSocket;

	qint32 blockSize;

	//qint32 friend_id;

	PersonalInfoStruct personalInfo;//大漠放了一个他自己定义的个人信息结构
	FriendInfoDetailDialog *friendInfoDlg;//这个是用来弹好友或者个人信息的窗口的

	SeekClientDialog *seekOther;//查找。
	AcceptAddFriendRequet *acceptAddFriendRequet;
	AcceptServerInfor *acceptServerInfor;

	/////////////////////////////////////////
	//新的聊天窗口
	QHash<qint32, SingleChatDialog*> hash_ID_chatDlg;
	QHash<qint32, FriendInfoDetailDialog*> hash_ID_InfoDetailDlg;
	QHash<int, QPixmap> hash_ID_headImage;
	QHash<int, QString> hash_ID_NickName;
	//QString nickName_self;
	qint32 id_src;
	qint32 id_des;
	QString html;
	QDateTime timeMessageSend;
	MessageRemindForm *messageRemindWidget;
	/////////////////////////
	QSystemTrayIcon *trayIcon;
	int messageCount;

	//这是同意添加好友的
	int requesterId;
	int friendId;
	QString time;

	//这部分是添加好友请求的请求者的相关信息。
	int transmitAddFriendId;
	int transmitAddSourceId;
	QString transmitAddAccount;
	QString transmitAddNickname;
	QString transmitAddTime;

	////////////////////////////////////////////////////
	//聊天信息相关变量
	qint32 imgCount;
	QByteArray imgTempDataByte;

	/////////////////////////////////////////////////////
	//传输文件
	qint64 fileSize;
	QString fileName;
	QByteArray iconByte;
	TransferFileRemindForm *transferFileRemindForm;
};
#endif
