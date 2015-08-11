#include "IKApplication.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	IKApplication w;
	w.show();
	return a.exec();
}
