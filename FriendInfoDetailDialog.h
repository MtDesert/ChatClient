#ifndef FRIENDINFODETAILDIALOG_H
#define FRIENDINFODETAILDIALOG_H

#include "ui_FriendInfoDetailDialog.h"
#include "Struct/PersonalInfoStruct.h"
#include "ImageLabel.h"
class FriendInfoDetailDialog : public QDialog, private Ui::FriendInfoDetailDialog
{
	Q_OBJECT

public:
	explicit FriendInfoDetailDialog(PersonalInfoStruct &friendInfo,bool allowEdit, QWidget *parent = 0);
	void updateGui(PersonalInfoStruct& info);
signals:
	void signalGetMyInfo();//请求服务端拿取自己的个人信息
	void signalSetMyInfo(PersonalInfoStruct &Info);//请求将自己的信息写回服务端
	void signalUpdateFriendInfo(qint32 id);//更新好友信息的信号
	void signalClose(qint32 id);
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void closeEvent(QCloseEvent *event);
private slots:
	void on_pushButtonBaseInfor_clicked();
	void on_pushButtonDetailInfor_clicked();
	void on_pushButtonUpdate_clicked();
	void on_pushButtonClose_clicked();
	void on_pushButtonOk_clicked();
private:
	void initSetting();
private:
	bool editing;//这是用来表示界面状态的
	PersonalInfoStruct myFriend;
	QPoint m_OffsetPosition;
};

#endif // FRIENDINFODETAILDIALOG_H
