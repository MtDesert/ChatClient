#ifndef REGISTERMOREINFORFORM_H
#define REGISTERMOREINFORFORM_H
#include <QDate>
#include "ui_RegisterMoreInforForm.h"

const int ANIMAL_START = 1900;

class RegisterMoreInforForm : public QWidget, private Ui::RegisterMoreInforForm
{//本类用于管理注册的更多信息窗口
	Q_OBJECT
	
public:
	explicit RegisterMoreInforForm(QDate date = QDate (), QWidget *parent = 0);
signals:
	void signalShowProcect();//弹出密保界面的信号
	//void signalRegInforList(const QStringList &QStringList, QPixmap headImage);
	void signalRegInfor(QByteArray &data);
private slots:
	void on_pushButtonPasswdProtect_clicked();
	void on_pushButtonOK_clicked();

private:
	void initSetting();	//初始化一些设定
	void initData();
	void initGui();
	void getGuiData();
	void constellations(int month, int day);
private:
	QString nickName;
	QString animalYear;
	QString constellation;
	QString blood;
	QString province;
	QString city;
	QString phone;
	QString mail;
	QPixmap headImagePix;
	QByteArray headImageByte;
	////////////////////////////
	QDate birthday;
	QVector<QString> animal;
	//QStringList regInforList;
};

#endif // REGISTERMOREINFORFORM_H
