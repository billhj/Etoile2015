#include "SkeletonViewer.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QFile stylesheet("./styles/stylesheet.txt");
	stylesheet.open(QFile::ReadOnly);
	QString setSheet = QLatin1String(stylesheet.readAll());
	a.setStyleSheet(setSheet);
	SkeletonViewer w;
	w.show();
	return a.exec();
}
