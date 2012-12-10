#ifndef SEEKCLIENTDIALOG_H
#define SEEKCLIENTDIALOG_H

#include "ui_seekclientdialog.h"
#include<QTcpSocket>
#include<QUdpSocket>
#include<QDate>
class QMouseEvent;
class SeekClientDialog : public QDialog, private Ui::SeekClientDialog
{
    Q_OBJECT

public:
    explicit SeekClientDialog(QWidget *parent = 0);
    void setSourseId(int id);
    void getTcpSocket(QTcpSocket *tcpSocket);
    void readFriendInformationFromServer(QDataStream &in);
signals:
    void signalSeekDlgClose();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private slots:

    void on_pushButton_seekfriend_clicked();

    void on_pushButton_seekFriendAdd_clicked();

    void on_pushButton_seekFriendForward_2_clicked();

    void on_pushButton_seekFriendForward_clicked();

    void on_pushButton_seekGroupOK_clicked();



    void on_pushButton_seekFriendCancel_clicked();

    void on_pushButton_seekFriendAddCancel_clicked();

private:
    //void connectToServer();
    void initDialog();


private:
    QString seekFriendUserNam;
    QString seekGroupName;
    int sourceID;//发送方的ID（即请求方）。
    qint32 nextBlockSize;
    QTcpSocket *tcpSocketSeek;
    QUdpSocket udpSocketConfigerAddFriend;

	int friendId;//这个初始化是在收到好友资料Id。

    QHostAddress severAddress;
    QString userName_seek;
    QString userNickname_seek;
    QString userSex_seek;
    QDate userBirthday_seek;
    QString userMail_seek;
    QString userMood_seek;
    QByteArray userHeadimage_seek;

    QPoint seekPostion;
};

#endif // SEEKCLIENTDIALOG_H
