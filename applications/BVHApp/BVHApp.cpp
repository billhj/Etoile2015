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

	
	
	//std::cout<<bvh.m_skeleton.m_jacobian<<std::endl;

	connect(ui.spinBox, SIGNAL(valueChanged(int)), this, SLOT(setMaxIterationsNb(int)));
	connect(ui.doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setDistanceThreshold(double)));
}

BVHApp::~BVHApp()
{

}

void BVHApp::addMenu()
{
	QMenuBar* bar = this->menuBar();
	QMenu* file = bar->addMenu("File");
	QAction* openBVH = file->addAction("BVH");
	connect(openBVH, SIGNAL(triggered()), this, SLOT(openBVH()));

	QMenu* ik = bar->addMenu("IK");
	QAction* jc = ik->addAction("JacobianCov");
	QAction* jdls = ik->addAction("JacobianDLS");
	QAction* jpi = ik->addAction("JacobianPseudoInverse");
	QAction* jt = ik->addAction("JacobianTranspose");
	connect(ik, SIGNAL(triggered(QAction*)), this, SLOT(applyIKAction(QAction*)));

	QMenu* anim = bar->addMenu("Animation");
	QAction* playbvh = anim->addAction("playBVH");
	connect(playbvh, SIGNAL(triggered()), this, SLOT(playBVH()));

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
		_pIKWidget->_pSolver = (new JacobianCov(_pIKWidget->_pSolver->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == "JacobianDLS")
	{
		_pIKWidget->_pSolver = (new JacobianDLSSolver(_pIKWidget->_pSolver->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == "JacobianPseudoInverse")
	{
		_pIKWidget->_pSolver = (new JacobianPseudoInverse(_pIKWidget->_pSolver->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == "JacobianTranspose")
	{
		_pIKWidget->_pSolver = (new JacobianTranspose(_pIKWidget->_pSolver->getMaxNumberOfTries()));
	}
	std::cout<<"now using : " <<t.toStdString()<<std::endl;
}

void BVHApp::openBVH()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("File (*.bvh)"));
	if(name.isEmpty()) return;
	bvh = BVH();
	bool b = bvh.loadFromBVHFile(name.toStdString());
	//bvh.m_skeleton.m_endeffectors.pop_back();
	//bvh.m_skeleton.m_endeffectors.pop_back();`
	if(b)
	{
		//bvh.m_skeleton.m_endeffectors.erase(bvh.m_skeleton.m_endeffectors.begin());
		//bvh.m_skeleton.m_endeffectors.erase(bvh.m_skeleton.m_endeffectors.begin());
		bvh.m_skeleton.m_endeffectors.erase(bvh.m_skeleton.m_endeffectors.begin());
		bvh.m_skeleton.buildJacobian(bvh.m_skeleton.m_endeffectors, bvh.m_skeleton.m_jacobian);
		bvh.m_skeleton.update();
		_pIKWidget->sk = &bvh.m_skeleton;
	}
}

void BVHApp::playBVH()
{
	_pIKWidget->animationframes = bvh.m_frames;
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
	_pIKWidget->_pSolver->setMaxNumberOfTries(i);
}

void BVHApp::setDistanceThreshold(double d)
{
	_pIKWidget->_pSolver->setTargetThreshold(d);
}

void BVHApp::updateCombobox()
{
	ui.comboBox->clear();
	/*for(int i = 0; i < _pIKWidget->sk->m_joints.size(); ++i)
	{
		ui.comboBox->addItem(QString().fromStdString(_pIKWidget->sk->m_joints[i].m_name));
	}*/
}

