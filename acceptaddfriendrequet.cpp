#include "acceptaddfriendrequet.h"

AcceptAddFriendRequet::AcceptAddFriendRequet(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

void AcceptAddFriendRequet::on_pushButton_AcceptAddFriendRefuse_clicked()
{
    this->close();
}

void AcceptAddFriendRequet::on_pushButton_AcceptAddFriendCloseWindow_clicked()
{
    this->close();
}

void AcceptAddFriendRequet::on_pushButton_AcceptAddFriendAgree_clicked()
{//同意添加对方添加为好友，这个操作将在数据库那边直接添加好友，用udp端口号是5888.
    QByteArray datagramAgreeAddFriend;
    QDataStream out(&datagramAgreeAddFriend, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);
    ////////acceptId被请求添加者，requesterId请求添加好友者。
    out<<qint32(0)<<9<<aadfriendId<<requesterId;

    out.device()->seek(0);
    out<<qint32(datagramAgreeAddFriend.size()- sizeof(datagramAgreeAddFriend));
	//qDebug()<<"here"<<9<<aadfriendId<<requesterId;
    tcpSocket->write(datagramAgreeAddFriend);
    qDebug()<<"agree add:"<<aadfriendId<<requesterId;
    this->close();
}

void AcceptAddFriendRequet::getTcpSocket(QTcpSocket *socket)
{
    tcpSocket = new QTcpSocket;
    tcpSocket = socket;
}

void AcceptAddFriendRequet::transmitAddFriendSetGui(int transmitAddSourceId, QString transmitAddAccount, QString transmitAddNickname, QString transmitAddTime)
{
    label_accceptAddFriendRequestShowTime->setText(transmitAddTime);
    label_AcceptAddFriendShowUserId->setText(QString::number(transmitAddSourceId));
    label_AcceptAddFriendShowUserName->setText(transmitAddAccount);
    label_AcceptAddFriendShowNickname->setText(transmitAddNickname);
}

void AcceptAddFriendRequet::setTwoUserID(int friendId, int requestId)
{
     aadfriendId = friendId;
     requesterId = requestId;
}



