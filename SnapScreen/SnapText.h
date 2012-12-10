#ifndef SNAPTEXT_H
#define SNAPTEXT_H

#include <QTextEdit>

class SnapText : public QTextEdit
{
	Q_OBJECT
/*
这个是一个用于输入的文本框,但它在这个截图功能中并为显示出来
而是接受焦点后即可输入,失去焦点后输入结束
*/
public:
	explicit SnapText(QWidget *parent = 0);
	~SnapText();
protected:
	void focusOutEvent(QFocusEvent *event);
signals:
	void signalFinishTyped();
};

#endif // SNAPTEXT_H