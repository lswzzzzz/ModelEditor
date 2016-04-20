#ifndef QTMODELEDITOR_H
#define QTMODELEDITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_qtmodeleditor.h"
#include "transdialog.h"

namespace Ui{
	class ParticleEditor;
}

class QTModelEditor : public QMainWindow
{
	Q_OBJECT
public:
	QTModelEditor(QWidget *parent = 0);
	~QTModelEditor();
	Ui::QTModelEditorClass getUI(){ return ui; }
	void addMainTrans();

private slots:
	
protected:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	
private:
	Ui::QTModelEditorClass ui;
	TransDialog* mainTrans;
};

#endif // QTMODELEDITOR_H
