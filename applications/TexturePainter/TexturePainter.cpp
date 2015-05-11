#include "TexturePainter.h"
#include "MeshViewer.h"
TexturePainter::TexturePainter(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	_pWidget = new MeshViewer();
	this->setCentralWidget(_pWidget);
}

TexturePainter::~TexturePainter()
{

}
