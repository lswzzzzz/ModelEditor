#include "CocosWidget.h"
#include <qdebug.h>
#include <qevent.h>

CocosWidget::CocosWidget(QWidget *parent)
	:QWidget(parent)
{
	setAcceptDrops(true);
	setObjectName("cocosWidget");
}

CocosWidget::~CocosWidget()
{
}

void CocosWidget::mouseMoveEvent(QMouseEvent *event)
{
	qDebug("mouseMoveEvent");
}

void CocosWidget::mousePressEvent(QMouseEvent *event)
{
	qDebug("mousePressEvent");
}

void CocosWidget::mouseReleaseEvent(QMouseEvent* event)
{
	qDebug("mouseReleaseEvent");
}


void CocosWidget::dragEnterEvent(QDragEnterEvent *event)
{
	/*ImageListWidget *source =
	qobject_cast<ImageListWidget *>(event->source());
	if (source && source != this) {
	event->setDropAction(Qt::MoveAction);
	event->accept();
	qDebug("dragEnterEvent");
	}*/
	event->acceptProposedAction();
	qDebug("YES");
}

void CocosWidget::dragMoveEvent(QDragMoveEvent *event)
{
	/*ImageListWidget *source =
	qobject_cast<ImageListWidget *>(event->source());
	if (source && source != this) {
	event->setDropAction(Qt::MoveAction);
	event->accept();
	qDebug("dragMoveEvent");
	}*/
	event->acceptProposedAction();
	qDebug("YES");
}

void CocosWidget::dropEvent(QDropEvent *event)
{
	/*ImageListWidget *source =
	qobject_cast<ImageListWidget *>(event->source());
	if (source && source != this) {
	addItem(event->mimeData()->text());
	event->setDropAction(Qt::MoveAction);
	event->accept();
	}*/
	/*event->acceptProposedAction();
	qDebug("dropEvent");*/
	event->acceptProposedAction();
	qDebug("YES");
}
