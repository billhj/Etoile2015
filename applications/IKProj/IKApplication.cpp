#include "IKApplication.h"

IKApplication::IKApplication(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	setWindowIcon(QIcon("./styles/icon.png"));
	_pIKWidget = new IKWidget();
	this->setCentralWidget(_pIKWidget);
	addMenu();
	this->setWindowTitle("IKViewer");

	connect(ui.spinBox, SIGNAL(valueChanged(int)), this, SLOT(setMaxIterationsNb(int)));
	connect(ui.doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setDistanceThreshold(double)));
}

IKApplication::~IKApplication()
{

}

void IKApplication::addMenu()
{
	QMenuBar* bar = this->menuBar();
	QMenu* ik = bar->addMenu("IK");
	QAction* MS = ik->addAction("^-^");
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

void IKApplication::applyIKAction(QAction* action)
{
	QString t = action->text();
	if(t.toStdString() == "^-^")
	{
		_pIKWidget->setIKSolver(new Etoile::JacobianRcompensedDLSSolver(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == ":)")
	{
		_pIKWidget->setIKSolver(new Etoile::JacobianDLSSVDSolver(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == "JacobianPseudoInverse")
	{
		_pIKWidget->setIKSolver(new Etoile::JacobianPseudoInverseSolver(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == "JacobianTranspose")
	{
		_pIKWidget->setIKSolver(new Etoile::JacobianTransposeSolver(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	else if(t.toStdString() == "JacobianDLS")
	{
		_pIKWidget->setIKSolver(new Etoile::JacobianDLSSolver(_pIKWidget->getIKSolver()->getMaxNumberOfTries()));
	}
	std::cout<<"now using : " <<t.toStdString()<<std::endl;
}

void IKApplication::openAbout()
{
	QMessageBox msgBox;
	msgBox.setText(" IKViewer \n Copyright(C) 2009-2012 \n author: ");
	//msgBox.setInformativeText("Do you want to save your changes?");
	//msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	//msgBox.setDefaultButton(QMessageBox::Save);
	int ret = msgBox.exec();
}

void IKApplication::openUsage()
{
	QMessageBox msgBox;
	msgBox.setText(" IKViewer \n Copyright(C) 2009-2012 \n author: \n 1: Shift + Right_Mouse: select the joint or two end effectors (wrist) \n 2: use the manipulator to apply IK");
	int ret = msgBox.exec();
}

void IKApplication::setMaxIterationsNb(int i)
{
	_pIKWidget->getIKSolver()->setMaxNumberOfTries(i);
}

void IKApplication::setDistanceThreshold(double d)
{
	_pIKWidget->getIKSolver()->setTargetThreshold(d);
}

void IKApplication::updateCombobox()
{
	ui.comboBox->clear();
	for(int i = 0; i < _pIKWidget->chain->m_joints.size(); ++i)
	{
		ui.comboBox->addItem(QString().fromStdString(_pIKWidget->chain->m_joints[i]->m_name));
	}
}

void IKApplication::changeComboboxItem(int index)
{
	int zIndex = _pIKWidget->chain->m_joints[index]->m_dims[0].m_idx;
	int yIndex = _pIKWidget->chain->m_joints[index]->m_dims[1].m_idx;
	int xIndex = _pIKWidget->chain->m_joints[index]->m_dims[2].m_idx;
	ui.zMin->setValue(_pIKWidget->chain->m_dim_anglelimites[zIndex](0)  * 180 / 3.14159265);
	ui.zMax->setValue(_pIKWidget->chain->m_dim_anglelimites[zIndex](1)  * 180 / 3.14159265);
	ui.yMin->setValue(_pIKWidget->chain->m_dim_anglelimites[yIndex](0)  * 180 / 3.14159265);
	ui.yMax->setValue(_pIKWidget->chain->m_dim_anglelimites[yIndex](1)  * 180 / 3.14159265);
	ui.xMin->setValue(_pIKWidget->chain->m_dim_anglelimites[xIndex](0)  * 180 / 3.14159265);
	ui.xMax->setValue(_pIKWidget->chain->m_dim_anglelimites[xIndex](1)  * 180 / 3.14159265);

	ui.lamdaZ->setValue(_pIKWidget->chain->m_posture_variation[zIndex]);
	ui.lamdaY->setValue(_pIKWidget->chain->m_posture_variation[yIndex]);
	ui.lamdaX->setValue(_pIKWidget->chain->m_posture_variation[xIndex]);
}

void IKApplication::setZMin(double v)
{
	int idx = _pIKWidget->chain->m_joints[ui.comboBox->currentIndex()]->m_dims[0].m_idx;
	_pIKWidget->chain->m_dim_anglelimites[idx](0) = v * 3.14159265 / 180.0;
	_pIKWidget->chain->updateAverageByMinMax(idx);
}

void IKApplication::setZMax(double v)
{
	int idx = _pIKWidget->chain->m_joints[ui.comboBox->currentIndex()]->m_dims[0].m_idx;
	_pIKWidget->chain->m_dim_anglelimites[idx](1) = v * 3.14159265 / 180.0;
	_pIKWidget->chain->updateAverageByMinMax(idx);
}

void IKApplication::setYMin(double v)
{
	int idx = _pIKWidget->chain->m_joints[ui.comboBox->currentIndex()]->m_dims[1].m_idx;
	_pIKWidget->chain->m_dim_anglelimites[idx](0) = v * 3.14159265 / 180.0;
	_pIKWidget->chain->updateAverageByMinMax(idx);
}

void IKApplication::setYMax(double v)
{
	int idx = _pIKWidget->chain->m_joints[ui.comboBox->currentIndex()]->m_dims[1].m_idx;
	_pIKWidget->chain->m_dim_anglelimites[idx](1) = v * 3.14159265 / 180.0;
	_pIKWidget->chain->updateAverageByMinMax(idx);
}

void IKApplication::setXMin(double v)
{
	int idx = _pIKWidget->chain->m_joints[ui.comboBox->currentIndex()]->m_dims[2].m_idx;
	_pIKWidget->chain->m_dim_anglelimites[idx](0) = v * 3.14159265 / 180.0;
	_pIKWidget->chain->updateAverageByMinMax(idx);
}

void IKApplication::setXMax(double v)
{
	int idx = _pIKWidget->chain->m_joints[ui.comboBox->currentIndex()]->m_dims[2].m_idx;
	_pIKWidget->chain->m_dim_anglelimites[idx](1) = v * 3.14159265 / 180.0;
	_pIKWidget->chain->updateAverageByMinMax(idx);
}

void IKApplication::setLamdaZ(double v)
{
	int idx = _pIKWidget->chain->m_joints[ui.comboBox->currentIndex()]->m_dims[0].m_idx;
	_pIKWidget->chain->m_posture_variation[idx] = v;
}

	
void IKApplication::setLamdaY(double v)
{
	int idx = _pIKWidget->chain->m_joints[ui.comboBox->currentIndex()]->m_dims[1].m_idx;
	_pIKWidget->chain->m_posture_variation[idx] = v;
}

void IKApplication::setLamdaX(double v)
{
	int idx = _pIKWidget->chain->m_joints[ui.comboBox->currentIndex()]->m_dims[2].m_idx;
	_pIKWidget->chain->m_posture_variation[idx] = v;
}
