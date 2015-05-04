#include "EtoileMeshViewer.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	EtoileMeshViewer w;
	w.show();
	return a.exec();
}
