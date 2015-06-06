#include "SkeletonViewer.h"
#include <QtGui/QApplication>
#include "animation/IK/IKChain.h"
#include "animation/IK/JacobianPseudoInverseSolver.h"
#include "animation/IK/JacobianTransposeSolver.h"
int main(int argc, char *argv[])
{
	//Etoile::IKChain chain("");
	//chain.loadFromFile("C:/Users/Jing/Documents/ikchain.ik");
	//JacobianPseudoInverseSolver solver;
	//solver.compute(&chain, Eigen::Vector3f(1,0,0), true);

	QApplication a(argc, argv);
	QFile stylesheet("./styles/stylesheet.txt");
	stylesheet.open(QFile::ReadOnly);
	QString setSheet = QLatin1String(stylesheet.readAll());
	a.setStyleSheet(setSheet);
	SkeletonViewer w;
	w.show();
	return a.exec();
}
