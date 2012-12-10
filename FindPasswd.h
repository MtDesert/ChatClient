#ifndef FINDPASSWD_H
#define FINDPASSWD_H

#include "ui_FindPasswd.h"
#include <QUdpSocket>
#include <QTcpSocket>
class QMouseEvent;

class FindPasswd : public QDialog, private Ui::FindPasswd
{
    Q_OBJECT

public:
    explicit FindPasswd(QWidget *parent = 0);
	void setLoginName(const QString &str);
private slots:
	void slotReadTcpFromServer();
	void slotConnectToHostReady();

    void on_lineEditUserName_textEdited(const QString &arg1);
    void on_pushButtonFind_clicked();

    void on_pushButtonConfigPasswd_clicked();
    void on_pushButtonClose_clicked();
    void on_pushButtonFindPasswd_clicked();

private:
    void senderUsernameToServer();
    void receiveQuestionsFromSever(QDataStream &in);
    void receiveAnserIsRight(QDataStream &in);
    void Init();//初始化一些变量
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
private:
    //从服务端获取的数据。
    QString q1;//问题
    QString q2;
    QString q3;

    QString a1;//答案
    QString a2;
    QString a3;

    QString userA1;//获取用户回答的问题。
    QString userA2;
    QString userA3;

    QString userNameFindPasswd;
	QTcpSocket *tcpSocket;
    qint32 messageType;//信息类型，辨别用户是否存在，是否设置密宝问题。
    bool clickFindPasswd;
    qint32 blockSize;
    QPoint m_OffsetPosition;

};

#endif // FINDPASSWD_H
