#ifndef PERSONINFOIMAGELABEL_H
#define PERSONINFOIMAGELABEL_H
#include "ImageLabel.h"
class QMouseEvent;
class QDropEvent;
class PersonInfoImageLabel : public ImageLabel
{//呢这个类派生自ImageLabel,为的是覆盖ImageLabel的mousePressEvent事件
	Q_OBJECT
public:
	explicit PersonInfoImageLabel(QWidget *parent = 0);
signals:
	void signalShowPersonInfoDlg();
	void signalUpdataPersonInforToServer();
public slots:

protected:
	void mousePressEvent(QMouseEvent *event);
	void dropEvent(QDropEvent *event);
private:
	
};

#endif // PERSONINFOIMAGELABEL_H
