#include <QtGui/QApplication>
#include "MyMainWindow.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QGLFormat glf = QGLFormat::defaultFormat();
    glf.setSampleBuffers(true);
    glf.setSamples(16);
    QGLFormat::setDefaultFormat(glf);

	MyMainWindow window;
	window.show();
	return a.exec();
}
