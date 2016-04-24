#include "BVHApp.h"
#include <QtGui/QApplication>
#include "BVH.h"

int main(int argc, char *argv[])
{
	BVH bvh;
	bvh.loadFromBVHFile("test.bvh");
	bvh.saveToBVHFile("test2.bvh");
	QApplication a(argc, argv);
	BVHApp w;
	w.show();
	return a.exec();
}
