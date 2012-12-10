#include "acceptserverinfor.h"

AcceptServerInfor::AcceptServerInfor(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

void AcceptServerInfor::readNotifyAgreeAddFriend(int requesterId, int friendId, QString time)
{
    label_acceptServerInforShowTime->setText(time);
    QString friendIdStr = QString::number(friendId);
    QString str1 = QString::fromUtf8("同意了你添加好友请求");
    QString str =friendIdStr + str1;
    textEdit_acceptInforShowInfor->setText(str);
}

void AcceptServerInfor::on_pushButton_acceptInforClose_clicked()
{
    this->close();
}

void AcceptServerInfor::on_pushButton_acceptInforOK_clicked()
{
    this->close();
}
