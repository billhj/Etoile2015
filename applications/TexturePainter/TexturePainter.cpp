#include "TexturePainter.h"
TexturePainter::TexturePainter(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	this->showMaximized();
	//connect(ui.actionShowAxis, SIGNAL(triggered()), ui.centralWidget, SLOT(showAxis()));
	//connect(ui.actionShowGrid, SIGNAL(changed()), ui.centralWidget, SLOT(showGrid()));
	//this->showFullScreen();
	//setWindowState(windowState() ^ Qt::WindowFullScreen);
}

TexturePainter::~TexturePainter()
{

}
