#include "BVHApp.h"
#include <QtGui/QApplication>


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	BVHApp w;
	w.show();
	return a.exec();
}
