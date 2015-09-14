#include "PointCloudApp.h"

PointCloudApp::PointCloudApp(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	_pWidget = new MeshViewer();
	this->setCentralWidget(_pWidget);
}

PointCloudApp::~PointCloudApp()
{

}
