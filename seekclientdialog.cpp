#include "seekclientdialog.h"
#include<QDebug>
#include<QHostAddress>
#include<QMessageBox>
#include<QMouseEvent>
SeekClientDialog::SeekClientDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    initDialog();
}

void SeekClientDialog::on_pushButton_seekfriend_clicked()
{
    label_seekUserNotExist->hide();
    seekFriendUserNam = lineEdit_seekFriend->text();
    if(seekFriendUserNam.length() <= 0)
    {
        label_seekUserNotExist ->setText(QString::fromUtf8("输入对方用户名或ID"));
        label_seekUserNotExist->show();
        return;
    }
    QByteArray seekBytes;
    QDataStream out(&seekBytes, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);

    int destinationID = 7;//表明发送的信息类型。
    //sourceID 查找请求者也是源。
    out<<qint32(0)<<destinationID<<sourceID<<seekFriendUserNam;
    out.device()->seek(0);
    out<<qint32(seekBytes.size() - sizeof(qint32));
    tcpSocketSeek->write(seekBytes);
    qDebug()<<"tcp";
}

void SeekClientDialog::readFriendInformationFromServer(QDataStream &in)
{
    in>>friendId;
    in>>userName_seek>>userNickname_seek>>userSex_seek>>userBirthday_seek
     >>userMail_seek>>userMood_seek>>userHeadimage_seek;
    qDebug()<<"ssss";
    qDebug()<<friendId<<userName_seek<<userNickname_seek<<userSex_seek<<userBirthday_seek
           <<userMail_seek<<userMood_seek;
    if(friendId == 0)
    {
        label_seekUserNotExist->setText(QString::fromUtf8("您查找的帐号不存在"));
        label_seekUserNotExist->show();
    }
    else
    {
        pushButton_seekfriend->setEnabled(false);
        this->resize(tabWidget->size());
        lineEdit_userFriendName->setText(userName_seek);
        lineEdit_userFriendNickname->setText(userNickname_seek);
        label_seekShowFriendSex->setText(userSex_seek);
        label_seekShowFriendBirthday->setText(userBirthday_seek.toString());
        label_seekShowFriendEmail->setText(userMail_seek);
        textEdit_seekShowFriendMood->setText(userMood_seek);

        if(userHeadimage_seek.length() > 0)
        {
            QPixmap pixmap;
            pixmap.loadFromData(userHeadimage_seek,"JEPG");
            label_seekFriendImage->setPixmap(pixmap.scaled(label_seekFriendImage->size()));
        }
    }
}

void SeekClientDialog::initDialog()
{
    label_seekUserNotExist->hide();
    label_seekGroupNotExist->hide();
    lineEdit_userFriendName->setReadOnly(true);
    lineEdit_userFriendNickname->setReadOnly(true);
    textEdit_seekShowFriendMood->setReadOnly(true);
    lineEdit_seekGroupName->setReadOnly(true);
    lineEdit_seekGroupNickname->setReadOnly(true);
    textEdit_seekShowGroupMood->setReadOnly(true);
    this->resize(widget_seekFriendTop->width(), widget_seekFriendTop->height() + 20);
}

void SeekClientDialog::on_pushButton_seekFriendAdd_clicked()
{
    if(sourceID == friendId)
    {
        this->close();
        QMessageBox::about(0, QString::fromUtf8("消息"), QString::fromUtf8("不能添添加自己为好友！"));
        return;
    }
    QByteArray datagramAddfriend;
    QDataStream out(&datagramAddfriend, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_7);

    int destination = 8;
    out<<quint32(0);
    out<<destination<<sourceID<<friendId;
    out.device()->seek(0);

    out<<quint32(datagramAddfriend.size() - sizeof(quint32));

    tcpSocketSeek->write(datagramAddfriend);
    qDebug()<<QString::fromUtf8("发送请求")<<destination<<sourceID<<friendId;
    emit signalSeekDlgClose();
    this->close();


    //发送到服务端，告诉服务器用户添加此好友。
}

void SeekClientDialog::on_pushButton_seekFriendForward_2_clicked()
{
    this->resize(widget_seekFriendTop->width(), widget_seekFriendTop->height() + 20);

}

void SeekClientDialog::on_pushButton_seekFriendForward_clicked()
{
    label_seekGroupNotExist->hide();
    this->resize(widget_seekFriendTop->width(), widget_seekFriendTop->height() + 20);
    pushButton_seekfriend->setEnabled(true);
}

void SeekClientDialog::on_pushButton_seekGroupOK_clicked()
{
    seekGroupName = lineEdit_seekGroupName->text();
    if(seekGroupName.length() == 0)
    {
        label_seekGroupNotExist->setText(QString::fromUtf8("请输入您要查找的群号"));
        label_seekGroupNotExist->show();
        return;
    }

    /////////////发送群号到服务端的代码/////////////////
}

void SeekClientDialog::setSourseId(int  Id)
{
    sourceID = Id;
}

void SeekClientDialog::getTcpSocket(QTcpSocket *tcpSocket)
{
    tcpSocketSeek = tcpSocket;
}

void SeekClientDialog::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        seekPostion = event->globalPos() - frameGeometry().topLeft();
    }
    else
    {
        QDialog::mousePressEvent(event);
    }
}

void SeekClientDialog::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        move(event->globalPos() - seekPostion);
    }
    else
    {
        QDialog::mouseMoveEvent(event);
    }
}



void SeekClientDialog::on_pushButton_seekFriendCancel_clicked()
{
    emit signalSeekDlgClose();
}

void SeekClientDialog::on_pushButton_seekFriendAddCancel_clicked()
{
    emit signalSeekDlgClose();
}
