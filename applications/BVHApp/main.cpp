#include "BVHApp.h"
#include <QtGui/QApplication>
#include "BVH.h"

int main(int argc, char *argv[])
{
	BVH bvh;
	bvh.loadFromBVHFile("Example1.bvh");
	bvh.saveToBVHFile("Example2.bvh");
	bvh.m_skeleton.buildJacobian(bvh.m_skeleton.m_endeffectors, bvh.m_skeleton.m_jacobian);
	std::cout<<bvh.m_skeleton.m_jacobian<<std::endl;
	QApplication a(argc, argv);
	BVHApp w;
	w.show();
	return a.exec();
}
