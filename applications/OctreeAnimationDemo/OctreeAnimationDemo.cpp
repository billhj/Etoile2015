#include "OctreeAnimationDemo.h"
#include <QFileDialog>

OctreeAnimationDemo::OctreeAnimationDemo(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	p_anim = new OctreeAnimation();
}

OctreeAnimationDemo::~OctreeAnimationDemo()
{

}


void OctreeAnimationDemo::loadFile()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("File (*.oc)"));
	//QFile file(name);
	if(name.isEmpty()) return;
	p_anim->loadOctree(name.toStdString());
}

void OctreeAnimationDemo::saveFile()
{
	QString name = QFileDialog::getSaveFileName(this, tr("save File"),"",tr("File (*.oc)"));
	//QFile file(name);
	if(name.isEmpty()) return;
	p_anim->saveOctree(name.toStdString());
}

void OctreeAnimationDemo::appendFile()
{
	QString name = QFileDialog::getOpenFileName(this, tr("append File"),"",tr("File (*.oc;*.bvh)"));
	//QFile file(name);
	if(name.isEmpty()) return;
	
	p_anim->addBVH(name.toStdString());

}

void OctreeAnimationDemo::saveTXT()
{
	QString name = QFileDialog::getSaveFileName(this, tr("save File"),"",tr("File (*.txt)"));
	//QFile file(name);
	if(name.isEmpty()) return;
	p_anim->saveMyOctree(name.toStdString());
}

void OctreeAnimationDemo::runBVH()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("File (*.bvh)"));
	//QFile file(name);
	if(name.isEmpty()) return;
	p_anim->solveOriginalTrajectory(0,2800,name.toStdString());
}