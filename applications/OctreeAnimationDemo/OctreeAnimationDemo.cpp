#include "OctreeAnimationDemo.h"
#include <QFileDialog>

OctreeAnimationDemo::OctreeAnimationDemo(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
}

OctreeAnimationDemo::~OctreeAnimationDemo()
{

}


void OctreeAnimationDemo::loadFile()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("File (*.oc)"));
	//QFile file(name);
	if(name.isEmpty()) return;
}

void OctreeAnimationDemo::saveFile()
{
	QString name = QFileDialog::getOpenFileName(this, tr("save File"),"",tr("File (*.oc)"));
	//QFile file(name);
	if(name.isEmpty()) return;
}

void OctreeAnimationDemo::appendFile()
{
	QString name = QFileDialog::getOpenFileName(this, tr("append File"),"",tr("File (*.oc)"));
	//QFile file(name);
	if(name.isEmpty()) return;
}