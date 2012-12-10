#ifndef CHATRECORDDIALOG_H
#define CHATRECORDDIALOG_H

#include "ui_ChatRecordDialog.h"
#include <QCalendarWidget>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

class ChatRecordDialog : public QDialog, private Ui::ChatRecordDialog
{
    Q_OBJECT

public:
    explicit ChatRecordDialog(QSqlDatabase chatDb, qint32 friend_id, qint32 self_id, QString friend_nick, QString self_nick, QWidget *parent = 0);
    void initData();
private slots:
    void on_pushButtonClose_clicked();

    void on_pushButtonDate_clicked();

    void slotSelectDate();

    void on_textEdit_textChanged();

    void on_pushButtonFirstPage_clicked();

    void on_pushButtonLeftPage_clicked();

    void on_pushButtonLastPage_clicked();

    void on_pushButtonRightPage_clicked();

protected:
    void initSet();
    void renovate();
private:
    QCalendarWidget *dateWidget;  //选择日期的Widget
    QSqlDatabase db;
    qint32 id_Friend;
    qint32 id_Self;
    QString nickName_Friend;
    QString nickName_Self;
    QString html;
    QString date;
    QString time;
    bool is_send;
    int page;
    int sum;

};

#endif // CHATRECORDDIALOG_H
