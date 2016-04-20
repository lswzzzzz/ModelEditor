#ifndef INFODIALOG_H
#define INFODIALOG_H

#include <QDialog>
#include "ui_TransDialog.h"

namespace Ui {
	class TransDialog;
}

class TransDialog : public QDialog
{
    Q_OBJECT

public:
	explicit TransDialog(QWidget *parent = 0);
	~TransDialog();


protected:
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
protected:
	void paintEvent(QPaintEvent*);
private:
	Ui::TransDialog *ui;
};

#endif // INFODIALOG_H
