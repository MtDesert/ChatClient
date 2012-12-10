#ifndef ACCEPTSERVERINFOR_H
#define ACCEPTSERVERINFOR_H

#include "ui_acceptserverinfor.h"
#include<QUdpSocket>
class AcceptServerInfor : public QDialog, private Ui::AcceptServerInfor
{
    Q_OBJECT

public:
    explicit AcceptServerInfor(QWidget *parent = 0);
    void readNotifyAgreeAddFriend(int requesterId, int friendId, QString time);
private slots:

    void on_pushButton_acceptInforClose_clicked();

    void on_pushButton_acceptInforOK_clicked();
};

#endif // ACCEPTSERVERINFOR_H
