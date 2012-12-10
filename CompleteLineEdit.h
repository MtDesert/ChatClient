#ifndef COMPLETELINEEDIT_H
#define COMPLETELINEEDIT_H

#include <QtGui/QLineEdit>
#include <QStringList>
#include <QHash>
#include <QStandardItem>
#include <QStandardItemModel>
class QListView;
class QStringListModel;
class QModelIndex;
class CompleteLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    CompleteLineEdit(QWidget *parent = 0);
    void dataFrom(const QStringList &words);
    void setHashData(QString str, QIcon img);
public slots:
	void slotSetCompleter(const QString &text); // 动态的显示完成列表
	void slotCompleteText(const QModelIndex &index); // 点击完成列表中的项，使用此项自动完成输入的单词
protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);
private:
    QStringList words; // 整个完成列表的单词
    QListView *listView; // 完成列表
    //QStringListModel *model; // 完成列表的model

    QHash<QString, QIcon> hashItemData;
    QStandardItemModel *model;
};
#endif
