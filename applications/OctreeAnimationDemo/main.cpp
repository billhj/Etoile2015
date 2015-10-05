#include "OctreeAnimationDemo.h"
#include <QtGui/QApplication>
#include "Octree.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OctreeAnimationDemo w;
	w.show();
	return a.exec();
}
