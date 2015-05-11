#include "TexturePainter.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TexturePainter w;
	w.show();
	return a.exec();
}
