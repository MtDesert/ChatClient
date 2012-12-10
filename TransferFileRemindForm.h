#ifndef TRANSFERFILEREMINDFORM_H
#define TRANSFERFILEREMINDFORM_H

#include "ui_TransferFileRemindForm.h"

const qint32 KB = 1024;
const qint32 MB = 1024 * KB;
const qint32 GB = 1024 * MB;

class TransferFileRemindForm : public QWidget, private Ui::TransferFileRemindForm
{
	Q_OBJECT
	
public:
	explicit TransferFileRemindForm(const QPixmap &img, QByteArray &fileIcon, const QString &nickName,
									qint32 id, qint64 size,  const QString &filename, QWidget *parent = 0);
private slots:
	void on_pushButtonClose_clicked();

private:
	void initSetting();
	void initGui();
private:
	QPixmap headimage;
	QByteArray fileIcon;
	QString friendNickName;
	qint32 id_friend;
	qint64 fileSize;
	QString fileName;
};

#endif // TRANSFERFILEREMINDFORM_H
