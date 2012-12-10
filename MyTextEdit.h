#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QTextEdit>

const int MESSAGE_SELF = 0;
const int MESSAGE_FRIEND = 1;

class MyTextEdit : public QTextEdit
{//本类主要就是实现聊天的文本输入框
	Q_OBJECT
public:
	explicit MyTextEdit(QWidget *parent = 0);
	void setInsertTextFormat(QFont font, QColor color);	//根据QFontDialog 和QColorDialog 的值来设定输入的内容格式
	void insertImage(const QString &path);
	void insertNickName_DateTime(const QString &nickName, const QDateTime &time, int type);
	void insertChatMessage(const QString &html);		//这个函数用于插入聊天的信息
signals:
	void signalPastePicture(QByteArray &byte);
public slots:
protected:
	void keyPressEvent(QKeyEvent *event);
private:
	void pastePicture(const QPixmap &tempPix);	//处理粘贴图片的函数
	void initData();
private:
	qint32 count;
};

#endif // MYTEXTEDIT_H
