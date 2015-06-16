#include "SkeletonViewer.h"
#include <QtGui/QApplication>
int main(int argc, char *argv[])
{
	/*Etoile::IKChain chain("");
	chain.loadFromFile("C:/Users/Jing/Documents/ikchain.ik");
	JacobianDLSSolver solver(&chain);
	solver.solve(Vector3_(1,1,1), true);*/

	QApplication a(argc, argv);
	QFile stylesheet("./styles/stylesheet.txt");
	stylesheet.open(QFile::ReadOnly);
	QString setSheet = QLatin1String(stylesheet.readAll());
	a.setStyleSheet(setSheet);
	SkeletonViewer w;
	w.show();
	return a.exec();
}
