#include "OctreeAnimationDemo.h"
#include <QtGui/QApplication>
#include "Octree.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OctreeAnimationDemo w;
	w.show();
	Octree o;
	o.load("");
	return a.exec();
}
