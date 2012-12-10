#include <QtGui/QApplication>
#include "LoginDialog.h"
#include <QTextCodec>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	LoginDialog w;
	w.show();
	return a.exec();
}
