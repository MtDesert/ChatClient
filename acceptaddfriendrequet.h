#ifndef ACCEPTADDFRIENDREQUET_H
#define ACCEPTADDFRIENDREQUET_H

#include "ui_acceptaddfriendrequet.h"
#include<QTcpSocket>
class AcceptAddFriendRequet : public QDialog, private Ui::AcceptAddFriendRequet
{
    Q_OBJECT

public:
    explicit AcceptAddFriendRequet(QWidget *parent = 0);
    void getTcpSocket(QTcpSocket *socket);
    void transmitAddFriendSetGui(int transmitAddSourceId, QString transmitAddAccount, QString transmitAddNickname, QString transmitAddTime);
    void setTwoUserID(int friendId, int requestId);
private slots:

    void on_pushButton_AcceptAddFriendRefuse_clicked();

    void on_pushButton_AcceptAddFriendCloseWindow_clicked();

    void on_pushButton_AcceptAddFriendAgree_clicked();

private:

    QTcpSocket *tcpSocket;
    int aadfriendId;//
    int requesterId; //请求者ID。
};

#endif // ACCEPTADDFRIENDREQUET_H
