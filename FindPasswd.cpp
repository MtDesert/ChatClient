#include "FindPasswd.h"
#include<QMessageBox>
#include<QMouseEvent>

#define USER_EXIST_PROTECT 1
#define USER_EXIST_NOT_PROTECT 0
#define USER_NOT_EXIST 2
FindPasswd::FindPasswd(QWidget *parent) :
	QDialog(parent)
{
	setupUi(this);
	widgetFindPasswd_Button->hide();
	pushButtonFind->setEnabled(false);
	tcpSocket = new QTcpSocket(this);
	tcpSocket->connectToHost(QHostAddress("127.0.0.1"), 8888);
	connect(tcpSocket, SIGNAL(connected()), this, SLOT(slotConnectToHostReady()));
	connect(tcpSocket, SIGNAL(readyRead()), this,SLOT(slotReadTcpFromServer()));

	this->setWindowFlags(Qt::FramelessWindowHint);
	this->resize(widgetFindPasswd_top->size());
	Init();
}

//发送用户的用户名或到服务器
void FindPasswd::senderUsernameToServer()
{
	QByteArray datagram;
	QDataStream out(&datagram, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_7);
	out << (qint32)0;
    out << (qint32)20;//20表示找回密码请求.
	out << (qint32)0;
	out << userNameFindPasswd;
	out.device()->seek(0);
	out << (qint32)(datagram.size() - (qint32)(sizeof(qint32)));
	tcpSocket->write(datagram);
}

void FindPasswd::on_lineEditUserName_textEdited(const QString &arg1)
{//b编辑帐户名的时候同时获取帐户名
	userNameFindPasswd = arg1;
}

//按下查找键。
void FindPasswd::on_pushButtonFind_clicked()
{//判断字符串是否是空的不要与""比较,要用lenght > 0
	if(userNameFindPasswd.length() > 0)
	{
		//lineEditUserNameWaiming->setText("waiting");
		labelUserNameWaiming->setText(QString("等待"));
		senderUsernameToServer();
		//qDebug()<<QString::fromUtf8("要取消udp链接了");
	}
}

void FindPasswd::slotReadTcpFromServer()
{
    qDebug()<<QString::fromUtf8("关于密码查找的受到服务端信息");
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_7);  
    int destinationId;
    if(0 == blockSize)
    {
        if(tcpSocket->bytesAvailable() < (qint32)sizeof(qint32))
        {
            return;
        }
        in >> blockSize;
    }
    if(tcpSocket->bytesAvailable() < blockSize)
    {
        qDebug()<<"here";
        qDebug()<<tcpSocket->bytesAvailable() ;
        return;
    }
    in>>destinationId;
    if(1 == destinationId)
    {
        receiveQuestionsFromSever(in);
    }
    else if(2 == destinationId)
    {
        receiveAnserIsRight(in);
    }
    else if(3 == destinationId)
    {
        int isSuccess;
        in>>isSuccess;
        if(1 == isSuccess)
        {
            this->close();
            QMessageBox::about(0, QString::fromUtf8("消息"), QString::fromUtf8("设置密码成功"));

        }
        else
        {
            QMessageBox::about(0, QString::fromUtf8("消息"), QString::fromUtf8("设置密码失败"));
        }

    }
    else
    {
        qDebug()<<QString::fromUtf8("服务端未知信息！！！");
    }
    blockSize = 0;
 }

void FindPasswd::Init()
{
	clickFindPasswd = false;
	lineEditPasswd->setEchoMode(QLineEdit::Password);
	lineEditConfigPasswd->setEchoMode(QLineEdit::Password);
	labelSetPasswdWarming->hide();
    labelFindPasswdTell_2->hide();
	groupBoxQuestion->hide();
	lineEditQuestion1->setReadOnly(true);
	lineEditQuestion2->setReadOnly(true);
	lineEditQuestion3->setReadOnly(true);
    pushButtonFind->setDefault(true);
}

void FindPasswd::on_pushButtonConfigPasswd_clicked()
{
	QString passwd = lineEditPasswd->text();
	QString configPasswd = lineEditConfigPasswd->text();
	if(passwd == configPasswd && passwd.length() > 0 && configPasswd.length() > 0)
	{
		labelSetPasswdWarming->hide();
		QByteArray passwdByte;
		QDataStream out(&passwdByte, QIODevice::WriteOnly);
		out << (qint32)0;
        out << 21;
        out <<0;
		out << passwd;

		out.device()->seek(0);
        out << (qint32)(passwdByte.size() - (qint32)sizeof(qint32));
        qDebug()<<passwd;
        //passwdByte.append(passwd);
		tcpSocket->write(passwdByte);

	}
	else
	{
        labelSetPasswdWarming->show();
	}
}

void FindPasswd::on_pushButtonClose_clicked()
{
	this->close();
}

void FindPasswd::mousePressEvent(QMouseEvent *event)
{//重载的鼠标按下
	if (event->button() == Qt::LeftButton)
	{
		m_OffsetPosition  = event->globalPos() - frameGeometry().topLeft();//获取屏幕左上角坐标与软件左上角坐标的差
	}
	else
	{
		QDialog::mousePressEvent(event);
	}

}

void FindPasswd::mouseMoveEvent(QMouseEvent *event)
{//重载的鼠标拖动
	if (event->buttons() == Qt::LeftButton)
	{
		move(event->globalPos() - m_OffsetPosition);
	}
	else
	{
		QDialog::mouseMoveEvent(event);
	}
}

void FindPasswd::setLoginName(const QString &str)
{
	lineEditUserName->setText(str);
	userNameFindPasswd = str;
}

void FindPasswd::slotConnectToHostReady()
{
	pushButtonFind->setEnabled(true);
}

void FindPasswd::on_pushButtonFindPasswd_clicked()
{
    labelFindPasswdTell_2->hide();
    a1 = lineEditAnswer1->text();
    a2 = lineEditAnswer2->text();
    a3 = lineEditAnswer3->text();
    if(a1.length() >0 && a2.length() >0 && a3.length() >0)
    {
        pushButtonFindPasswd->setEnabled(false);
        QByteArray byte;
        QDataStream out(&byte, QIODevice::WriteOnly);
        //out.setDevice(QDataStream::Qt_4_7);
        out<<(qint32)0;
        out<<22;//22表示目的
        out<<0;//这里的源是用户的ID。
        out<<userNameFindPasswd;
        out<<a1<<a2<<a3;
        out.device()->seek(0);
        out << (qint32)(byte.size() - (qint32)sizeof(qint32));
        tcpSocket->write(byte);
    }
    else
    {
        labelFindPasswdTell_2->setText(QString::fromUtf8("还没有填写完答案"));
        labelFindPasswdTell_2->show();
    }
}

void FindPasswd::receiveQuestionsFromSever(QDataStream &in)
{
    in>>messageType;
    in>>q1>>q2>>q3;
    if(USER_EXIST_PROTECT == messageType)
    {
        pushButtonFindPasswd->setDefault(true);
        pushButtonFind->setDefault(false);
        labelUserNameWaiming->hide();
        pushButtonFindPasswd->setEnabled(true);
        //显示密保问题。
        lineEditQuestion1->setText(q1);
        lineEditQuestion2->setText(q2);
        lineEditQuestion3->setText(q3);
        this->resize(widgetFindPasswd_top->width() ,widgetFindPasswd_middle->height() +widgetFindPasswd_top->height() );
        groupBoxQuestion->show();
    }
    else if(USER_EXIST_NOT_PROTECT == messageType)
    {
        this->resize(widgetFindPasswd_top->size());
        labelUserNameWaiming->setText(QString::fromUtf8("用户没有设置密宝问题"));
        labelUserNameWaiming->show();
    }
    else if(USER_NOT_EXIST == messageType)
    {
        this->resize(widgetFindPasswd_top->size());
        labelUserNameWaiming->setText(QString::fromUtf8("用户不存在"));
        labelUserNameWaiming->show();
    }
    else
    {
        this->resize(widgetFindPasswd_top->size());
        labelUserNameWaiming->setText(QString::fromUtf8("查询发生未知错误"));
        labelUserNameWaiming->show();
    }
    qDebug()<<"find"<<messageType<<q1<<q2<<q3;
}

void FindPasswd::receiveAnserIsRight(QDataStream &in)
{
    labelFindPasswdTell_2->show();
    qDebug()<<"is";
    int answerIsRight;
    in>>answerIsRight;
    qDebug()<<answerIsRight;
    if(1 == answerIsRight)
    {
        pushButtonFindPasswd->setDefault(false);
        pushButtonConfigPasswd->setDefault(true);
        labelFindPasswdTell_2->setText(QString::fromUtf8("密码成功找回,请在下面重新输入密码"));
        this->resize(widgetFindPasswd_top->width(), widgetFindPasswd_top->height() + widgetFindPasswd_middle->height() +widgetFindPasswd_Button->height());
        widgetFindPasswd_Button->show();
    }
    else
    {
        labelFindPasswdTell_2->setText(QString::fromUtf8("答案有误,请重新输入"));
        pushButtonFindPasswd->setEnabled(true);
    }
}
