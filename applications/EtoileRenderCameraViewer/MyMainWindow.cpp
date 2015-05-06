#include "MyMainWindow.h"

MyMainWindow::MyMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	_pWidget = new MeshViewer();
	this->setCentralWidget(_pWidget);
}

MyMainWindow::~MyMainWindow()
{

}
