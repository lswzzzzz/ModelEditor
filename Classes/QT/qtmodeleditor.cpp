#include "qtmodeleditor.h"
#include "Global.h"

QTModelEditor::QTModelEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	g_editor = this;
	setMouseTracking(true);
}

QTModelEditor::~QTModelEditor()
{

}

void QTModelEditor::mousePressEvent(QMouseEvent *event)
{
	
}

void QTModelEditor::mouseMoveEvent(QMouseEvent *event)
{
	int a = 1;
}

void QTModelEditor::addMainTrans()
{
	mainTrans = new TransDialog(this);
	mainTrans->show();
	QRect rect = this->geometry();
	QRect rect2 = ui.centralWidget->geometry();
	QRect rect3 = ui.SceneWidgetContents->geometry();
	QRect rectMain = QRect(rect.left(), rect.top() + rect2.top() + rect3.top(), ui.SceneWidget->rect().right(), rect3.bottom() - rect3.top());
	mainTrans->setGeometry(rectMain);
}