#ifndef MYLISTVIEW_H
#define MYLISTVIEW_H

#include <QListView>
const int OFFSET_VIEW = 10;
class MyListView : public QListView
{
	Q_OBJECT
public:
	explicit MyListView(QWidget *parent = 0);
	
signals:
	
private slots:
protected:
	void mouseMoveEvent(QMouseEvent *event);
private:
	void initSetting();
private:

	
};

#endif // MYLISTVIEW_H
