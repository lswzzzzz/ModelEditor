#include "transdialog.h"
#include "Global.h"
#include "QPainter"

TransDialog::TransDialog(QWidget *parent) :
    QDialog(parent),
	ui(new Ui::TransDialog)
{
    ui->setupUi(this);
	QPalette pal = palette();
	pal.setColor(QPalette::Background, QColor(0x00, 0xff, 0x00, 0x00));
	setPalette(pal);
	this->setAttribute(Qt::WA_TranslucentBackground, true);
	this->setWindowOpacity(0);
}

TransDialog::~TransDialog()
{
    delete ui;
}

void TransDialog::mousePressEvent(QMouseEvent *e)
{
	addConsoleInfo("YES");
}

void TransDialog::mouseMoveEvent(QMouseEvent *e)
{

}

void TransDialog::paintEvent(QPaintEvent*)
{
	/*QPainter p(this);
	p.setCompositionMode(QPainter::CompositionMode_Clear);
	p.fillRect(10, 10, 300, 300, Qt::SolidPattern);*/
}