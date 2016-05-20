#include "BVHApp.h"
#include <QtGui/QApplication>


int main(int argc, char *argv[])
{
	//Eigen::initParallel();
	QApplication a(argc, argv);
	QGLFormat glf = QGLFormat::defaultFormat();
    glf.setSampleBuffers(true);
    glf.setSamples(16);
    QGLFormat::setDefaultFormat(glf);
	BVHApp w;
	w.show();
	return a.exec();
}
