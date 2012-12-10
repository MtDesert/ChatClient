#include "CompleteLineEdit.h"
#include <QPushButton>
#include <QKeyEvent>
#include <QtGui/QListView>
#include <QtGui/QStringListModel>
#include <QDebug>
CompleteLineEdit::CompleteLineEdit(QWidget *parent)
    : QLineEdit(parent)
{
    listView = new QListView(this);
    model = new QStandardItemModel(this);
    listView->setWindowFlags(Qt::ToolTip);
    connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(slotSetCompleter(const QString &)));
    connect(listView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(slotCompleteText(const QModelIndex &)));
}
void CompleteLineEdit::focusOutEvent(QFocusEvent *e)
{
    if(e->lostFocus())
    {
        listView->hide();
    }
}
void CompleteLineEdit::keyPressEvent(QKeyEvent *e)
{
    if (!listView->isHidden())
    {
		int count = listView->model()->rowCount();	//一共有多少项
		QModelIndex currentIndex = listView->currentIndex();

		switch(e->key())
		{
			case Qt::Key_Down:
			{// 按向下方向键时，移动光标选中 下一个 完成列表中的项
				//qDebug()<<QString::fromUtf8("按下方向键下");
				int row = currentIndex.row() + 1;
				if (row >= count)
				{
					row = 0;
				}
				QModelIndex index = listView->model()->index(row, 0);
				listView->setCurrentIndex(index);
				break;
			}
			case Qt::Key_Up:
			{// 按向下方向键时，移动光标选中 上一个 完成列表中的项
				//qDebug()<<QString::fromUtf8("按下方向键上");
				int row = currentIndex.row() - 1;
				if (row < 0)
				{
					row = count - 1;
				}
				QModelIndex index = listView->model()->index(row, 0);
				listView->setCurrentIndex(index);
				break;
			}
			case Qt::Key_Escape:
			{// 按下Esc键时，隐藏完成列表
				listView->hide();
				break;
			}
			case Qt::Key_Return:
			{// 按下回车键时，使用完成列表中选中的项，并隐藏完成列表
				if (currentIndex.isValid())
				{
					QString text = listView->currentIndex().data().toString();
					setText(text);
				}
				listView->hide();
				qDebug()<<QString::fromUtf8("在这里调出流云的对话框~~");
				break;
			}
			default:
			{// 其他情况，隐藏完成列表，并使用QLineEdit的键盘按下事件
				listView->hide();
				QLineEdit::keyPressEvent(e);
				break;
			}
		}//end of switch
    }
    else
	{//嗯把其他的东西扔给基类总是明知的选择
        QLineEdit::keyPressEvent(e);
    }
}
void CompleteLineEdit::slotSetCompleter(const QString &text)
{
    if (text.isEmpty())
    {
        listView->hide();
        return;
    }
    if ((text.length() > 1) && (!listView->isHidden()))
    {
        return;
    }

    // 如果完整的完成列表中的某个单词包含输入的文本，则加入要显示的完成列表串中
    QStringList sl;
    foreach(QString word, words)
    {
        if (word.contains(text))
        {
            sl << word;
        }
    }
    model->clear();
    foreach (QString word, sl)
    {
        QIcon icon = hashItemData.value(word);
        if(icon.isNull())
        {
            qDebug()<<"icon is null!!!!!!!!"<<endl;
        }
//		qDebug()<<QString::fromUtf8("生成数据项")<<endl;
		QStandardItem *item = new QStandardItem(icon, word);
		item->setData(123456);

        model->appendRow(item);
    }

    listView->setModel(model);

    if (0 == model->rowCount())
    {
        return;
    }

    // Position the text edit
    listView->setMinimumWidth(width());
    listView->setMaximumWidth(width());

    QPoint p(0, height());
    int x = mapToGlobal(p).x();
    int y = mapToGlobal(p).y() + 1;

    listView->move(x, y);
    listView->show();
}

void CompleteLineEdit::slotCompleteText(const QModelIndex &index)
{
    QString text = index.data().toString();
    setText(text);
	/*
	QIcon icon = index.data(Qt::DecorationRole).value<QIcon>();
	QPushButton *p = new QPushButton;
	p->setIcon(icon);
	p->show();
	*/
	int i = index.data(Qt::UserRole + 1).value<int>();
	qDebug()<<text << i;
    listView->hide();
}

void CompleteLineEdit::dataFrom(const QStringList &words)
{
    this->words = words;
//	qDebug()<<QString::fromUtf8("补完的列表")<<words;
}

void CompleteLineEdit::setHashData(QString str, QIcon img)
{
	if(!hashItemData.contains(str))
	{
//		qDebug()<<QString::fromUtf8("不是重复的项可以插入");
		qDebug()<<str;
		hashItemData.insert(str, img);
		qDebug()<<hashItemData;
		words << str;
	}
	else
	{
//			qDebug()<<QString::fromUtf8("重复的项目不能插入!!");
	}
}
