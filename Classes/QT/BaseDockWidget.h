#pragma once
#include "cocos2d.h"
#include <qdockwidget.h>
#include "QToolButton"

class BaseDockWidget : public QDockWidget
{
	Q_OBJECT
public:
	BaseDockWidget(char* title, QSize size, bool isNeed = false);
	~BaseDockWidget();
	QDockWidget* getDockWidget();

	void NeedMinOrMaxToolButton();
private slots:
	void on_minButton_clicked();
protected:
	QWidget* widget;
	QToolButton *minButton;
	QSize size;
};

