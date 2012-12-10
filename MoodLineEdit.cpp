#include "MoodLineEdit.h"
#include <QPainter>
#include <QEvent>
MoodLineEdit::MoodLineEdit(QWidget *parent)
	: QAbstractButton( parent )
{
	setText("");
	setToolTip("");
	lineEdit = new QLineEdit("", this );
	lineEdit->hide();

	lineEdit->installEventFilter( this );

	this->setSizePolicy( lineEdit->sizePolicy() );

	connect( this, SIGNAL(clicked()), this, SLOT(slotEditStarted()) );
	connect( lineEdit, SIGNAL(editingFinished()), this, SLOT(slotEditFinished()) );
}
QSize MoodLineEdit::sizeHint() const
{
	return lineEdit->sizeHint();
}

QSize MoodLineEdit::minimumSize() const
{
	return lineEdit->minimumSize();
}

QLineEdit* MoodLineEdit::edit() const
{
	return lineEdit;
}

void MoodLineEdit::setMoodString(const QString &str)
{
	strMood = str;
}

void MoodLineEdit::paintEvent(QPaintEvent *e)
{
	QPainter p( this );
	QFontMetrics fm = p.fontMetrics();
	if ( underMouse() )
	{
		p.save();
		p.setPen( QPen( Qt::lightGray ));
		p.drawRect( this->rect().adjusted( 1, 1, -1, -1 ) ) ;
		p.restore();
	}
	p.drawText( this->rect().adjusted( 2, 1, -1, -1 ),
				Qt::AlignVCenter,
				fm.elidedText( lineEdit->text().isEmpty() ? strMood : lineEdit->text(),
							   Qt::ElideRight, this->rect().width() ));
}

void MoodLineEdit::resizeEvent(QResizeEvent *e)
{
	lineEdit->setGeometry( rect() );
}

bool MoodLineEdit::eventFilter(QObject *obj, QEvent *event)
{
	if ( obj == lineEdit )
	{
		if ( event->type() == QEvent::FocusOut )
		{
			slotEditFinished();
			event->accept();
			return true;
		}
	}

	return QAbstractButton::eventFilter( obj, event );
}

void MoodLineEdit::slotEditStarted()
{
	lineEdit->show();
	lineEdit->selectAll();
	lineEdit->setFocus();
	this->setToolTip( "" );
}

void MoodLineEdit::slotEditFinished()
{
	strMood = lineEdit->text();
	this->setText(strMood);
	lineEdit->hide();
	this->setToolTip(strMood);
	emit signalMoodString(strMood);
}
