#include "BVHApp.h"
#include "BVH.h"
BVH bvh;

BVHApp::BVHApp(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	setWindowIcon(QIcon("./styles/icon.png"));
	_pIKWidget = new SkeletonWidget();
	this->setCentralWidget(_pIKWidget);
	addMenu();
	this->setWindowTitle("SkeletonViewer");

	
	bvh.loadFromBVHFile("Example1.bvh");
	bvh.saveToBVHFile("Example2.bvh");
	bvh.m_skeleton.buildJacobian(bvh.m_skeleton.m_endeffectors, bvh.m_skeleton.m_jacobian);
	bvh.m_skeleton.update();
	_pIKWidget->sk = &bvh.m_skeleton;
	std::cout<<bvh.m_skeleton.m_jacobian<<std::endl;

	connect(ui.spinBox, SIGNAL(valueChanged(int)), this, SLOT(setMaxIterationsNb(int)));
	connect(ui.doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setDistanceThreshold(double)));
}

BVHApp::~BVHApp()
{

}

void BVHApp::addMenu()
{
	QMenuBar* bar = this->menuBar();
	QMenu* ik = bar->addMenu("IK");
	QAction* MS = ik->addAction("JacobianCov");
	QAction* CCD = ik->addAction(":)");
	QAction* PI = ik->addAction("JacobianPseudoInverse");
	QAction* TRS = ik->addAction("JacobianTranspose");
	QAction* DLS = ik->addAction("JacobianDLS");
	connect(ik, SIGNAL(triggered(QAction*)), this, SLOT(applyIKAction(QAction*)));

	QMenu* help = bar->addMenu("Help");
	QAction* usage = help->addAction("How to use");
	QAction* about = help->addAction("About IKViewer");
	connect(about, SIGNAL(triggered()), this, SLOT(openAbout()));
	connect(usage, SIGNAL(triggered()), this, SLOT(openUsage()));
	updateCombobox();
}

void BVHApp::applyIKAction(QAction* action)
{
	QString t = action->text();
	if(t.toStdString() == "JacobianCov")
	{
		//_pIKWidget->setIKSolver(new Etoile::JacobianCov(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == ":)")
	{
		//_pIKWidget->setIKSolver(new Etoile::JacobianDLSSVDSolver(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == "JacobianPseudoInverse")
	{
		//_pIKWidget->setIKSolver(new Etoile::JacobianPseudoInverseSolver(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == "JacobianTranspose")
	{
		//_pIKWidget->setIKSolver(new Etoile::JacobianTransposeSolver(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == "JacobianDLS")
	{
		//_pIKWidget->setIKSolver(new Etoile::JacobianDLSSolver(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	std::cout<<"now using : " <<t.toStdString()<<std::endl;
}

void BVHApp::openAbout()
{
	QMessageBox msgBox;
	msgBox.setText(" IKViewer \n Copyright(C) 2009-2012 \n author: ");
	//msgBox.setInformativeText("Do you want to save your changes?");
	//msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	//msgBox.setDefaultButton(QMessageBox::Save);
	int ret = msgBox.exec();
}

void BVHApp::openUsage()
{
	QMessageBox msgBox;
	msgBox.setText(" IKViewer \n Copyright(C) 2009-2012 \n author: \n 1: Shift + Right_Mouse: select the joint or two end effectors (wrist) \n 2: use the manipulator to apply IK");
	int ret = msgBox.exec();
}

void BVHApp::setMaxIterationsNb(int i)
{
	//_pIKWidget->getIKSolver()->setMaxNumberOfTries(i);
}

void BVHApp::setDistanceThreshold(double d)
{
	//_pIKWidget->getIKSolver()->setTargetThreshold(d);
}

void BVHApp::updateCombobox()
{
	ui.comboBox->clear();
	/*for(int i = 0; i < _pIKWidget->sk->m_joints.size(); ++i)
	{
		ui.comboBox->addItem(QString().fromStdString(_pIKWidget->sk->m_joints[i].m_name));
	}*/
}

