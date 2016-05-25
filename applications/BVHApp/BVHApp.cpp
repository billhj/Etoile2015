/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BVHApp.cpp
* @brief 
* @date 1/2/2011
*/
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
	connect(ui.timelinewidget, SIGNAL(activeFrameChanged(int)), this, SLOT(frameIndexChanged(int)));
	connect(ui.damping1, SIGNAL(valueChanged(double)), this, SLOT(changeDamping(double)));
	connect(ui.damping2, SIGNAL(valueChanged(double)), this, SLOT(changeDamping(double)));
	mode = false;

	_pIKWidget->m_parameter.distance_threshold = ui.doubleSpinBox->value();
	_pIKWidget->m_parameter.max_iterations = ui.spinBox->value();
	_pIKWidget->m_parameter.damping1 = ui.damping1->value();
	_pIKWidget->m_parameter.damping2 = ui.damping2->value();
	_pIKWidget->m_parameter.usingroot = ui.usingRoot->isChecked();
	_pIKWidget->_pSolver = (new JacobianDLSSolver(_pIKWidget->m_parameter.max_iterations, _pIKWidget->m_parameter.distance_threshold, _pIKWidget->m_parameter.damping1));
	_pIKWidget->_pSolver->setRootActive(_pIKWidget->m_parameter.usingroot);
	connect(ui.usingRoot, SIGNAL(stateChanged(int)), this, SLOT(setRoot(int)));
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
	QAction* gp = ik->addAction("GaussianProcess");
	connect(ik, SIGNAL(triggered(QAction*)), this, SLOT(applyIKAction(QAction*)));

	QMenu* anim = bar->addMenu("Animation");
	QAction* playbvh = anim->addAction("playBVH");
	QAction* stopbvh = anim->addAction("stopBVH");
	QAction* accbvh = anim->addAction("+");
	QAction* deaccbvh = anim->addAction("-");
	anim->addSeparator();


	connect(playbvh, SIGNAL(triggered()), this, SLOT(playBVH()));
	connect(stopbvh, SIGNAL(triggered()), this, SLOT(stopBVH()));
	connect(accbvh, SIGNAL(triggered()), this, SLOT(accBVH()));
	connect(deaccbvh, SIGNAL(triggered()), this, SLOT(deaccBVH()));


	QAction* normalmode = anim->addAction("Normal");
	QAction* sequencemode = anim->addAction("sequenceRegenerating");
	QAction* sequenceEditMode = anim->addAction("sequentialEditing");
	QAction* postureEditMode = anim->addAction("postureEditing");

	actionGroup = new QActionGroup(this);
	actionGroup->addAction(normalmode);
	actionGroup->addAction(sequencemode);
	actionGroup->addAction(sequenceEditMode);
	actionGroup->addAction(postureEditMode);

	QAction* generate = anim->addAction("generate");
	QAction* saveBVH = anim->addAction("saveBVH");

	normalmode->setCheckable(true);
	sequencemode->setCheckable(true);
	sequenceEditMode->setCheckable(true);
	postureEditMode->setCheckable(true);
	normalmode->setChecked(true);
	//connect(actionGroup, SIGNAL(triggered(QAction *)), this, SLOT(changeMode(QAction *)));
	connect(normalmode, SIGNAL(triggered()), this, SLOT(setMode0()));
	connect(sequencemode, SIGNAL(triggered()), this, SLOT(setMode1()));
	connect(sequenceEditMode, SIGNAL(triggered()), this, SLOT(setMode2()));
	connect(postureEditMode, SIGNAL(triggered()), this, SLOT(setMode3()));
	connect(generate, SIGNAL(triggered()), this, SLOT(generateSequence()));
	connect(saveBVH, SIGNAL(triggered()), this, SLOT(saveGenerateSequence()));



	QMenu* help = bar->addMenu("Help");
	QAction* usage = help->addAction("How to use");
	QAction* about = help->addAction("About IKViewer");
	connect(about, SIGNAL(triggered()), this, SLOT(openAbout()));
	connect(usage, SIGNAL(triggered()), this, SLOT(openUsage()));
	updateCombobox();
}


void BVHApp::setRoot(int b)
{
	_pIKWidget->m_parameter.usingroot = ui.usingRoot->isChecked();
	_pIKWidget->_pSolver->setRootActive(_pIKWidget->m_parameter.usingroot);
}

void BVHApp::applyIKAction(QAction* action)
{
	QString t = action->text();
	if(t.toStdString() == "JacobianCov")
	{
		_pIKWidget->_pSolver = (new JacobianCov(_pIKWidget->m_parameter.max_iterations, _pIKWidget->m_parameter.distance_threshold, _pIKWidget->m_parameter.damping1, _pIKWidget->m_parameter.damping2));
	}
	else if(t.toStdString() == "JacobianDLS")
	{
		_pIKWidget->_pSolver = (new JacobianDLSSolver(_pIKWidget->m_parameter.max_iterations, _pIKWidget->m_parameter.distance_threshold, _pIKWidget->m_parameter.damping1));
	}
	else if(t.toStdString() == "JacobianPseudoInverse")
	{
		_pIKWidget->_pSolver = (new JacobianPseudoInverse(_pIKWidget->m_parameter.max_iterations, _pIKWidget->m_parameter.distance_threshold, _pIKWidget->m_parameter.damping1));
	}
	else if(t.toStdString() == "JacobianTranspose")
	{
		_pIKWidget->_pSolver = (new JacobianTranspose(_pIKWidget->m_parameter.max_iterations, _pIKWidget->m_parameter.distance_threshold, _pIKWidget->m_parameter.damping1));
	}
	else if(t.toStdString() == "GaussianProcess")
	{
		if(_pIKWidget->sk==NULL) return;
		_pIKWidget->_pSolver = new GPIKsolver(_pIKWidget->sk);
		_pIKWidget->_pSolver->setMaxNumberOfTries(1);
	}
	_pIKWidget->_pSolver->setRootActive(_pIKWidget->m_parameter.usingroot);
	std::cout<<"now using : " <<t.toStdString()<<std::endl;
}

void BVHApp::openBVH()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("File (*.bvh)"));
	if(name.isEmpty()) return;
	bvh = BVH();
	bool b = bvh.loadFromBVHFile(name.toStdString());

	if(b)
	{
		//bvh.m_skeleton.m_endeffectors.erase(bvh.m_skeleton.m_endeffectors.begin());
		//bvh.m_skeleton.m_endeffectors.erase(bvh.m_skeleton.m_endeffectors.begin());
		//bvh.m_skeleton.m_endeffectors.erase(bvh.m_skeleton.m_endeffectors.begin());
		//bvh.m_skeleton.m_endeffectors.pop_back();
		//bvh.m_skeleton.m_endeffectors.pop_back();
		bvh.m_skeleton.buildJacobian(bvh.m_skeleton.m_endeffectors, bvh.m_skeleton.m_jacobian);
		bvh.m_skeleton.update();
		_pIKWidget->sk = &bvh.m_skeleton;
		_pIKWidget->m_gp.setSK(_pIKWidget->sk);
		_pIKWidget->animationframes = bvh.m_frames;
	}
}

void BVHApp::playBVH()
{
	//_pIKWidget->animationframes.clear();
	//_pIKWidget->animationframes = bvh.m_frames;
	ui.timelinewidget->startTimer();
}

void BVHApp::stopBVH()
{
	ui.timelinewidget->stopTimer();
}

void BVHApp::accBVH()
{
	ui.timelinewidget->setSpeed(ui.timelinewidget->getSpeed() + 1);
}

void BVHApp::deaccBVH()
{
	ui.timelinewidget->setSpeed(ui.timelinewidget->getSpeed() - 1);
	if(ui.timelinewidget->getSpeed() <= 0) ui.timelinewidget->setSpeed(0);
}

void BVHApp::frameIndexChanged(int index)
{
	_pIKWidget->_frameIdx = index;
	if(index>=0)
	{
		//_pIKWidget->pos = (bvh.m_frames[index].m_values);

		if(mode==2)
		{
			int start = _pIKWidget->_frameIdx - _pIKWidget->windowSize;
			if(start < 0)
			{
				start = 0;
			}
			int end = _pIKWidget->_frameIdx + _pIKWidget->windowSize;
			if(_pIKWidget->animationframes.size() < end)
			{
				end = _pIKWidget->animationframes.size();
			}
			if(index < end && index > start)
			{
				bvh.m_skeleton.m_dim_values = bvh.m_frames[index].m_values;
				bvh.m_skeleton.update();
				//std::cout<<index<< " 1 "<<bvh.m_skeleton.m_dim_values[0]<<" "<<bvh.m_skeleton.m_dim_values[1] <<" "<<bvh.m_skeleton.m_dim_values[2]<<std::endl;
				std::vector<Vector3_>& target = _pIKWidget->animationframes[index].m_targets;
				{
					VectorX_ t = VectorX_::Zero(target.size() * 3);
					for(unsigned int j = 0 ; j < target.size(); ++j)
					{
						Matrix3_ orientation = bvh.m_skeleton.m_joint_globalOrientations[0];
						Vector3_ pos = bvh.m_skeleton.m_joint_globalPositions[0];
						Vector3_ cur = orientation.transpose() * (target[j] - pos);
						t(j * 3) = cur(0);
						t(j * 3 + 1) = cur(1);
						t(j * 3 + 2) = cur(2);
					}
					JacobianCov* sol = dynamic_cast<JacobianCov*>(_pIKWidget->_pSolver);
					if(sol != NULL)
					{
						TargetGaussian tg = _pIKWidget->m_gp.computeASample(t);
						sol->setParameters(tg.m_invcov, tg.m_mu);
					}else{

						GPIKsolver* sol2 = dynamic_cast<GPIKsolver*>(_pIKWidget->_pSolver);
						if(sol2 != NULL)
						{
							//sol2->computeASample(t);
							TargetGaussian tg = _pIKWidget->m_gp.computeASample(t);
							sol2->setMU(tg.m_mu);
						}
						JacobianDLSSolver* sol3 = dynamic_cast<JacobianDLSSolver*>(_pIKWidget->_pSolver);
						if(sol3 != NULL)
						{
							_pIKWidget->sk->resetRotationValues();
						}
					}
					//bvh.m_skeleton.resetNotRootRotationValues();
					_pIKWidget->_pSolver->solve(&(bvh.m_skeleton), target);
					_pIKWidget->animationframes[index].m_values = bvh.m_skeleton.m_dim_values;
					//std::cout<<index<< " 2 "<<bvh.m_skeleton.m_dim_values[0]<<" "<<bvh.m_skeleton.m_dim_values[1] <<" "<<bvh.m_skeleton.m_dim_values[2]<<std::endl;
				}
			}

		}


	}else
	{
		bvh.m_skeleton.resetValues();
		//_pIKWidget->pos = bvh.m_skeleton.m_dim_values;
	}

	/*if(!mode)
	{
	_pIKWidget->animationframes.clear();
	if(bvh.m_frames.size() > index && index > 0)
	_pIKWidget->animationframes.push_back(bvh.m_frames[index]);
	else if(index < 0)
	{
	bvh.m_skeleton.resetValues();
	}
	else
	{
	bvh.m_skeleton.resetValues();
	}
	}
	else
	{
	_pIKWidget->animationframes.clear();
	if(_generatedFrame.size() > index && index > 0)
	_pIKWidget->animationframes.push_back(_generatedFrame[index]);
	else if(index < 0)
	{
	bvh.m_skeleton.resetValues();
	}else
	{
	bvh.m_skeleton.resetValues();
	}
	}*/
}

void BVHApp::changeMode()
{
	_pIKWidget->mode = mode;
	if(mode == 0)
	{
		_pIKWidget->animationframes = bvh.m_frames;
	}
	else if(mode == 1)
	{
		_pIKWidget->animationframes = _generatedFrame;
	}else if(mode == 2)
	{
		_pIKWidget->animationframes = bvh.m_frames;
		_pIKWidget->originalframes = bvh.m_frames;;
	}
	else if(mode == 3)
	{
		_pIKWidget->animationframes.clear();
	}
}

void BVHApp::changeDamping(double)
{
	{
		JacobianCov* solver = dynamic_cast<JacobianCov*>(_pIKWidget->_pSolver);
		if(solver != NULL)
		{
			solver->m_dampling1 = ui.damping1->value();
			solver->m_dampling2 = ui.damping2->value();
		}
	}
	{
		JacobianDLSSolver* solver = dynamic_cast<JacobianDLSSolver*>(_pIKWidget->_pSolver);
		if(solver != NULL)
		{
			solver->m_dampling = ui.damping1->value();
		}
	}
}

void BVHApp::openAbout()
{
	QMessageBox msgBox;
	msgBox.setText(" IKViewer \n Copyright(C) 2009-2016 \n author: Jing HUANG");
	//msgBox.setInformativeText("Do you want to save your changes?");
	//msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	//msgBox.setDefaultButton(QMessageBox::Save);
	int ret = msgBox.exec();
}

void BVHApp::openUsage()
{
	QMessageBox msgBox;
	msgBox.setText(" IKViewer \n Copyright(C) 2009-2016 \n author: \n 1: double click Right_Mouse: select the joint or two end effectors (wrist) \n 2: use the manipulator to apply IK");
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


void BVHApp::saveGenerateSequence()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save"), "./", tr("BVH Files (*.bvh)"));
	if(!mode) 
	{
		std::vector<Frame> temp = bvh.m_frames;
		for(unsigned int i = 0; i < bvh.m_frames.size(); ++i)
		{
			std::vector<double>& f = bvh.m_frames[i].m_values;
			f[0] = 0;
			f[1] = 0;
			f[2] = 0;
			f[3] = 0;
			f[4] = 0;
			f[5] = 0;
		}
		bvh.saveToBVHFile(fileName.toStdString());
		bvh.m_frames = temp;
	}else
	{
		std::vector<Frame> temp = bvh.m_frames;
		bvh.m_frames = _generatedFrame;
		bvh.saveToBVHFile(fileName.toStdString());
		bvh.m_frames = temp;
	}
}

void BVHApp::generateSequence()
{

	if(mode == 0)
	{
		_pIKWidget->animationframes = bvh.m_frames;
	}
	else if(mode == 1)
	{

		for(unsigned int i = 0; i < bvh.m_frames.size(); ++i)
		{
			std::vector<double> f = bvh.m_frames[i].m_values;
			f[0] = 0;
			f[1] = 0;
			f[2] = 0;
			f[3] = 0;
			f[4] = 0;
			f[5] = 0;
			bvh.m_skeleton.m_dim_values = f;
			bvh.m_skeleton.update();
			_generatedFrame.push_back(Frame());
			Frame& fr = _generatedFrame.back();
			std::vector<Vector3_> target;
			for(unsigned int j = 0; j < bvh.m_skeleton.m_endeffectors.size(); ++j)
			{
				int idx = bvh.m_skeleton.m_endeffectors[j];
				target.push_back(bvh.m_skeleton.m_joint_globalPositions[idx]);
			}
			fr.m_targets = (target);
			//
		}

		double dis = 0;
		bvh.m_skeleton.resetValues();


		QElapsedTimer timer;
		timer.start();

		for(unsigned int i = 0; i < _generatedFrame.size(); ++i)
		{
			std::vector<Vector3_>& target = _generatedFrame[i].m_targets;
			{
				VectorX_ t = VectorX_::Zero(target.size() * 3);
				for(unsigned int j = 0 ; j < target.size(); ++j)
				{
					Matrix3_ orientation = bvh.m_skeleton.m_joint_globalOrientations[0];
					Vector3_ pos = bvh.m_skeleton.m_joint_globalPositions[0];
					Vector3_ cur = orientation.transpose() * (target[j] - pos);
					t(j * 3) = cur(0);
					t(j * 3 + 1) = cur(1);
					t(j * 3 + 2) = cur(2);
				}
				JacobianCov* sol = dynamic_cast<JacobianCov*>(_pIKWidget->_pSolver);
				if(sol != NULL)
				{
					TargetGaussian tg = _pIKWidget->m_gp.computeASample(t);
					sol->setParameters(tg.m_invcov, tg.m_mu);
				}

				GPIKsolver* sol2 = dynamic_cast<GPIKsolver*>(_pIKWidget->_pSolver);
				if(sol2 != NULL)
				{
					//sol2->computeASample(t);
					TargetGaussian tg = _pIKWidget->m_gp.computeASample(t);
					sol2->setMU(tg.m_mu);
				}
				JacobianDLSSolver* sol3 = dynamic_cast<JacobianDLSSolver*>(_pIKWidget->_pSolver);
				if(sol3 != NULL)
				{
					_pIKWidget->sk->resetRotationValues();
				}
			}

			_pIKWidget->_pSolver->solve(&(bvh.m_skeleton), target);
			dis += _pIKWidget->_pSolver->getDistance();
			_generatedFrame[i].m_values = bvh.m_skeleton.m_dim_values;
		}

		qint64 nano = timer.nsecsElapsed();
		double speed = nano * 0.000001;
		dis/= _generatedFrame.size();
		std::cout<<"Solver: "<< _pIKWidget->_pSolver->getIKSolverName()<<std::endl;
		std::cout<<"Sequence: "<<bvh.name()<<  " size: "<< _generatedFrame.size() <<" frames  " << speed <<" msc"<< " tdistance: "<<dis<<std::endl;
		double error = 0;
		double it = 0;
		for(unsigned int i = 0; i < bvh.m_frames.size(); ++i)
		{
			std::vector<double>& f = bvh.m_frames[i].m_values;
			std::vector<double>& f1 = _generatedFrame[i].m_values;
			for(unsigned int j = 6; j < _generatedFrame[i].m_values.size(); ++j)
			{
				error += abs(f[j] - f1[j]);
				it = it + 1;
			}
		}
		error /= it;
		std::cout<< "msn: "<<error<<std::endl;
		_pIKWidget->animationframes = _generatedFrame;
	}
	else if(mode == 2)
	{

		int start = _pIKWidget->_frameIdx - _pIKWidget->windowSize;
		if(start < 0)
		{
			start = 0;
		}
		int end = _pIKWidget->_frameIdx + _pIKWidget->windowSize;
		if(_pIKWidget->animationframes.size() < end)
		{
			end = _pIKWidget->animationframes.size();
		}
		bvh.m_skeleton.m_dim_values = bvh.m_frames[start].m_values;
		bvh.m_skeleton.update();
		for(unsigned int i = start; i < end; ++i)
		{
			std::vector<Vector3_>& target = _pIKWidget->animationframes[i].m_targets;
			{
				VectorX_ t = VectorX_::Zero(target.size() * 3);
				for(unsigned int j = 0 ; j < target.size(); ++j)
				{
					Matrix3_ orientation = bvh.m_skeleton.m_joint_globalOrientations[0];
					Vector3_ pos = bvh.m_skeleton.m_joint_globalPositions[0];
					Vector3_ cur = orientation.transpose() * (target[j] - pos);
					t(j * 3) = cur(0);
					t(j * 3 + 1) = cur(1);
					t(j * 3 + 2) = cur(2);
				}
				JacobianCov* sol = dynamic_cast<JacobianCov*>(_pIKWidget->_pSolver);
				if(sol != NULL)
				{
					TargetGaussian tg = _pIKWidget->m_gp.computeASample(t);
					sol->setParameters(tg.m_invcov, tg.m_mu);
				}

				GPIKsolver* sol2 = dynamic_cast<GPIKsolver*>(_pIKWidget->_pSolver);
				if(sol2 != NULL)
				{
					//sol2->computeASample(t);
					TargetGaussian tg = _pIKWidget->m_gp.computeASample(t);
					sol2->setMU(tg.m_mu);
				}
				JacobianDLSSolver* sol3 = dynamic_cast<JacobianDLSSolver*>(_pIKWidget->_pSolver);
				if(sol3 != NULL)
				{
					_pIKWidget->sk->resetRotationValues();
				}
			}
			bvh.m_skeleton.m_dim_values = bvh.m_frames[i].m_values;
			_pIKWidget->_pSolver->solve(&(bvh.m_skeleton), target);
			_pIKWidget->animationframes[i].m_values = bvh.m_skeleton.m_dim_values;
		}
	}

}