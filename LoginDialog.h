#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "ui_LoginDialog.h"
#include <QFileInfoList>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include "XmlReader.h"
#include "XmlWriter.h"
#include "FriendListDialog.h"
#include "RegisterDialog.h"
#include "FindPasswd.h"
class QMouseEvent;
class QCompleter;
class QDir;

class LoginDialog : public QDialog, private Ui::LoginDialog
{
	Q_OBJECT

public:
	explicit LoginDialog(QWidget *parent = 0);
signals:

protected:
	void mousePressEvent(QMouseEvent *event);//拖动用的
	void mouseMoveEvent(QMouseEvent *event); //拖动用的
private slots:
	void on_pushButtonClose_clicked();
	void on_comboBoxName_textChanged(const QString &arg1);
	void on_lineEditPasswd_textChanged(const QString &arg1);
	void on_pushButtonLogin_clicked();
	void on_pushButtonRegister_clicked();
	void on_checkBoxRemember_clicked(bool checked);
	void on_pushButtonFind_clicked();

	void slotConnectToHostReady();
	void slotReadTcpDatagramFromServer();

private:
	bool readFile(const QString fileName);
	void getCompleteName();//实现填充名字自动补完的列表
	void getUserInfoString(); //这个函数是把用户所输入的信息都以字符串返回,便于写入到本地xml中
	void writePersonInfoIntoLocal();
	void autoFull(const QString &str);
	void initSetting();
private:
	QPoint m_OffsetPosition;

	QString userName_login;
	QString passwd_login;
	QString passwdHash;
	QString loginResult;
	QString userIdFromServer;

	int state;
	bool rememberPasswd;
	bool autoLogin;

	QFileInfoList fileNameList;//包括./和../的文件名
	QStringList strFileNameList;//user里的所有文件名
	QString filePath;
	QStringList list;
	QCompleter *completer;
	QStringList personInfo;
	QPoint chatLeftPostion;

	XmlReader xmlReader;
	XmlWriter xmlWriter;

	FriendListDialog *friendListDlg;
	RegisterDialog *registerDlg;

	QTcpSocket *tcpSocket;

	QDir *p_dir;

	FindPasswd *findPasswdDlg;


};

#endif // LOGINDIALOG_H
