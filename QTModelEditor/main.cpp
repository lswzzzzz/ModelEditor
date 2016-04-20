#include "QTModelEditor.h"
#include <QtWidgets/QApplication>
#include "Global.h"
#include "AppDelegate.h"
#include "QScrollArea"
#include "CocosWidget.h"
#include <qplaintextedit.h>

int main(int argc, char *argv[])
{
	CCQApplication::addLibraryPath("./plugins");
	AppDelegate app(argc, argv);
	QTModelEditor w;
	w.show();
	RECT t;

	logText = dynamic_cast<QPlainTextEdit*>(w.getUI().LogWidgetContents->children().at(0));
	QRect qr = w.getUI().SceneWidgetContents->rect();
	t.left = qr.left();
	t.right = qr.right();
	t.top = qr.top();
	t.bottom = qr.bottom();
	CocosWidget* widget = new CocosWidget();
	glWidget = widget;
	widget->setMinimumSize(QSize(qr.right(), qr.bottom()));
	w.getUI().SceneWidget->setWidget(widget);
	//setWidget
	t.right = 960;
	t.bottom = 640;
	app.setParent((HWND)widget->winId(), t);
	g_app = &app;

	//http://blog.sina.com.cn/s/blog_3e62c50d010118iy.html 
	return cocos2d::CCQApplication::getInstance()->run();
}
