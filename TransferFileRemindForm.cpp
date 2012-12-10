#include "TransferFileRemindForm.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
TransferFileRemindForm::TransferFileRemindForm(const QPixmap &img, QByteArray &fileIcon, const QString &nickName,
											   qint32 id, qint64 size,  const QString &filename, QWidget *parent) :
	headimage(img), fileIcon(fileIcon), friendNickName(nickName), id_friend(id), fileSize(size), fileName(filename), QWidget(parent)
{
	setupUi(this);
	initSetting();
	initGui();
}

void TransferFileRemindForm::initSetting()
{
	this->setWindowFlags(Qt::FramelessWindowHint);		//无边框
	this->setAttribute(Qt::WA_TranslucentBackground, true);//背景透明
	int x = (qApp->desktop()->width() - this->width());
	int y = (qApp->desktop()->height());
	this->move(x, y);
}

void TransferFileRemindForm::initGui()
{
	labelHeadImage->setPixmap(headimage.scaled(labelHeadImage->size()));
	QPixmap icon;
	icon.loadFromData(fileIcon, "PNG");
	labelIcon->setPixmap(icon.scaled(labelIcon->size()));
	labelNickName_ID->setText(friendNickName + "(" + QString::number(id_friend) + ")");
	labelFileName->setText(fileName);

    QString fileSizeStr;
    if(fileSize > 0 && fileSize < KB)
    {//0~kb
        fileSizeStr = QString::number(fileSize) + " bytes";
    }
    else if(fileSize >= KB && fileSize < MB )
    {//kb~mb
        fileSizeStr = QString::number((float)fileSize / KB) + " Kb";
    }
    else if(fileSize >= MB && fileSize < GB)
    {//mb~gb
        fileSizeStr = QString::number((float)fileSize / MB) + " Mb";
    }
    else if(fileSize >= GB && fileSize < 4 * GB)
    {//gb~4gb(32bit size limit)
        fileSizeStr = QString::number((float)fileSize / GB) + " Gb";
    }
    else
    {// >= 4gb
        qDebug()<<"file size over 4Gb!!!!!!";
        fileSizeStr = QString::number((float)fileSize / GB) + " Gb";
    }


    label->setText(QString("向您发送文件")  + " " + fileSizeStr);
	label->adjustSize();
}

void TransferFileRemindForm::on_pushButtonClose_clicked()
{
	this->close();
}
