#include "PointCloudApp.h"
#include "PointCloudXYZ.h"
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

void PointCloudApp::openFile()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("File (*.pcd; *.sk; *.bvh)"));
	if(name.isEmpty()) return;
	PointCloudXYZ* cloud = new PointCloudXYZ();
	cloud->readFromFile(name.toStdString());
	cloud->setVisible(true);
}