#pragma once
#include <qwidget.h>

class CocosWidget : public QWidget
{
	Q_OBJECT
public:
	CocosWidget(QWidget *parent = 0);
	~CocosWidget();
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent* event);
	void dragEnterEvent(QDragEnterEvent *event);
	void dragMoveEvent(QDragMoveEvent *event);
	void dropEvent(QDropEvent *event);
};

