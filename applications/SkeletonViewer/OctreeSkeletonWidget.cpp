#include "OctreeSkeletonWidget.h"
#include <QFileDialog>
#include "util/File.h"
OctreeSkeletonWidget::OctreeSkeletonWidget(QWidget *parent)
	: QWidget(parent), octreeskeleton(NULL)
{
	ui.setupUi(this);
	octreeskeleton = new OctreeSkeleton();
}

OctreeSkeletonWidget::~OctreeSkeletonWidget()
{

}


void OctreeSkeletonWidget::openOctreeSkeletonFiles()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("File (*.obj; *.sk; *.bvh)"));
	//QFile file(name);
	if(name.isEmpty()) return;

	std::string fpath = Etoile::File::getFilePath(name.toStdString());
	std::string fname = Etoile::File::getFileNameWithoutExtension(name.toStdString());
	octreeskeleton->reload(fpath+fname);
	emit filesOpened();
}

void OctreeSkeletonWidget::setDrawType(int type)
{
	octreeskeleton->drawType = type;
}

void OctreeSkeletonWidget::setDepth(int depth)
{
	octreeskeleton->depth = depth;
}