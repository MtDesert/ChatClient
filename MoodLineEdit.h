#ifndef MOODLINEEDIT_H
#define MOODLINEEDIT_H

#include <QtGui/QAbstractButton>
#include <QLineEdit>
class MoodLineEdit : public QAbstractButton
{
	Q_OBJECT
	
public:
	MoodLineEdit(QWidget* parent = 0);
	QSize sizeHint() const;
	QSize minimumSize() const;
	QLineEdit* edit() const;
	void setMoodString(const QString &str);
signals:
	void signalMoodString(const QString &str);	//当心情更变的时候发出此信号
protected:
	void paintEvent(QPaintEvent *e);
	void resizeEvent(QResizeEvent *e );
	bool eventFilter(QObject *obj, QEvent *event );

public slots:
	void slotEditStarted();
	void slotEditFinished();

private:
	QLineEdit* lineEdit;
	QString strMood;
};

#endif
