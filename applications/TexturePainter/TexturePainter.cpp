#include "TexturePainter.h"
TexturePainter::TexturePainter(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	this->showMaximized();
	//ui.comboBox.ad
}

TexturePainter::~TexturePainter()
{

}
