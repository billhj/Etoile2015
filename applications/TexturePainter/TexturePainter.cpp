#include "TexturePainter.h"
TexturePainter::TexturePainter(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	this->showMaximized();
	//this->showFullScreen();
	//setWindowState(windowState() ^ Qt::WindowFullScreen);
}

TexturePainter::~TexturePainter()
{

}
