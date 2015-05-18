#include "TexturePainter.h"
#include <qlabel.h>

TexturePainter::TexturePainter(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	this->showMaximized();
	p_colordlg = new QColorDialog(this);
	p_colordlg->setWindowTitle(QColorDialog::tr("Select color"));
	ui.colorButton->setAutoFillBackground(true);
	QString styleSheet = QString("background-color: rgb(0, 0, 0);");
	ui.colorButton->setStyleSheet(styleSheet);
}

TexturePainter::~TexturePainter()
{

}


void TexturePainter::selectColor()
{
	QPalette pal = ui.colorButton->palette();
	QColor input = pal.color(QPalette::Button);
	p_colordlg->setCurrentColor(input);
	int resultCode = p_colordlg->exec();
	QColor result;
	if (resultCode == QDialog::Accepted)
		result = p_colordlg->currentColor();
	QString styleSheet = QString("background-color: rgb(%1, %2, %3);").arg(result.red()).arg(result.green()).arg(result.blue());
	ui.colorButton->setStyleSheet(styleSheet);
}

void TexturePainter::pipetteColorPicked()
{
	QColor result;
	result.setRgbF(ui.centralWidget->m_pixel[0], ui.centralWidget->m_pixel[1], ui.centralWidget->m_pixel[2]);
	QString styleSheet = QString("background-color: rgb(%1, %2, %3);").arg(result.red()).arg(result.green()).arg(result.blue());
	ui.colorButton->setStyleSheet(styleSheet);
}