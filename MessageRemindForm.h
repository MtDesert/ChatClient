#ifndef MESSAGEREMINDFORM_H
#define MESSAGEREMINDFORM_H

#include "ui_MessageRemindForm.h"
#include <QListView>
#include <QStandardItemModel>
#include <QStandardItem>
#include "MyListView.h"
class MessageRemindForm : public QWidget, private Ui::MessageRemindForm
{
	Q_OBJECT
	
public:
	explicit MessageRemindForm(QWidget *parent = 0);
	void addMessage(int id, QPixmap headimage, QString str, QString chatHtml);
	//void deleteObjectLater();	//恩本来打算在这里调用deleteLater的没有达到预期
signals:
	void signalClickedItem(QModelIndex index);
	void signalMessageReminFormClose(const MessageRemindForm *p);
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void focusOutEvent(QFocusEvent *event);
private slots:
	void slotClickedItem(const QModelIndex &index);

private:
	void initSetting();
	void initData();
private:
	QStandardItemModel *model;
	 MyListView *view;
	QPoint offset;
	int count;	//用于统计消息的条数
};

#endif // MESSAGEREMINDFORM_H
