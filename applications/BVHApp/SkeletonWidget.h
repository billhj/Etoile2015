/**
* Copyright(C) 2009-2012                
* @author Jing HUANG 
* @file SkeletonWidget.h
* @brief 
* @date 1/2/2011
*/

#pragma once
#include "renderer/OpenGL/glhead.h"
#include "renderer/OpenGL/glDrawFunctions.h"
#include <QGLViewer/qglviewer.h>
#include <QMainWindow>
#include <QString>
#include <QTime>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QSize>
#include <QTimer>
#include "Manipulator.h"
#include "Skeleton.h"
#include <QTime>
#include "renderer/OpenGL/glfunctions.h"
#include <QElapsedTimer>
#include "JacobianDLSSolver.h"
#include "JacobianCov.h"
#include "JacobianPseudoInverse.h"
#include "JacobianTranspose.h"
#include "GPIKsolver.h"
#include "BVH.h"
#include "GaussianProcess.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

struct SolverParamter
{
	int max_iterations;
	double distance_threshold;
	double damping1;
	double damping2;
	bool usingroot;
};


class SkeletonWidget : public QGLViewer
{
	Q_OBJECT

		//static const int WIDGETWIDTH = 800, WIDGETHEIGHT = 600;
public:
	int framesNb;
	SkeletonWidget(QMainWindow* parent = 0): _pParent(parent), _scaleFactor(5), _selectedJointIndex(-1)
	{
		framesNb = 0;
		sk = NULL;
		speed = 0;
		_manipulator.reset();
		_manipulator.setOrigine(qglviewer::Vec(0, 0, 0));
	}

	~SkeletonWidget(){}



	void drawInfo(int currentJoint)
	{
		if(currentJoint < 0) return;
		Matrix3_ matrix = sk->m_joint_globalOrientations[currentJoint];
		Vector3_ pos = sk->m_joint_globalPositions[currentJoint];
		float mat[4][4];
		for(int w = 0; w < 3; ++w)
			for(int h = 0; h < 3; ++h)
			{
				mat[w][h] = matrix(h,w);
			}
			mat[3][0] = 0;
			mat[3][1] = 0;
			mat[3][2] = 0;
			mat[3][3] = 1;
			mat[0][3] = 0;
			mat[1][3] = 0;
			mat[2][3] = 0;

			glPushMatrix();
			glTranslatef(pos(0),pos(1),pos(2));
			glMultMatrixf(&mat[0][0]);
			QGLViewer::drawAxis(0.1); // Or any scale
			glPopMatrix();

			glLineWidth(0.3);
			glColor3f(1,0.4,0.7);
			qglviewer::Vec screenPos = camera()->projectedCoordinatesOf(qglviewer::Vec(pos.x(), pos.y(), pos.z()));
			drawText((int)screenPos[0]+20, (int)screenPos[1]-10, QString(" %4: [ %1, %2, %3 ] ").arg(pos.x(), 0, 'f',2).arg(pos.y(), 0, 'f',2).arg(pos.z(), 0, 'f',2).arg(QString(sk->m_joints[currentJoint].m_name.c_str())) );

	}

	void drawPlane()
	{
		glLineWidth(0.5);
		glColor3f(0.0,0.8,0.8);
		glPushMatrix();
		//glBegin(GL_QUADS);
		//glNormal3d(0,1,0);
		///*glVertex3f(5000,(_boundingBox.minimum().y() - 0.1) * _scaleFactor,50000);
		//glVertex3f(5000,(_boundingBox.minimum().y() - 0.1) * _scaleFactor,-50000);
		//glVertex3f(-5000,(_boundingBox.minimum().y() - 0.1) * _scaleFactor,-50000);
		//glVertex3f(-5000,(_boundingBox.minimum().y() - 0.1) * _scaleFactor,50000);*/
		//glVertex3f(5000,0,50000);
		//glVertex3f(5000,0,-50000);
		//glVertex3f(-5000,0,-50000);
		//glVertex3f(-5000,0,50000);
		//glEnd();
		//_board.draw();
		glColor3f(0.1,0.1,0.1);
		glRotated(90,1,0,0);
		drawGrid(10,40);		
		glPopMatrix();
	}

	void drawCurrentAxis(Etoile::Vec3f pos, Etoile::Quaternionf q)
	{
		glPushMatrix();
		double length = 5.0 *_scaleFactor;
		Etoile::Vec3f xAxis(1,0,0);
		Etoile::Vec3f yAxis(0,1,0);
		Etoile::Vec3f zAxis(0,0,1);
		glLineWidth(3);
		Etoile::Vec3f angle = q.getEulerAngleXYZBYAngle();
		glTranslatef(pos.x(),pos.y(),pos.z());
		xAxis = q * xAxis;
		yAxis = q * yAxis;
		zAxis = q * zAxis;
		/*glPushMatrix();
		glColor4f(1,0,0,1);
		glBegin(GL_LINES);
		glVertex3f(xAxis.x() * length , xAxis.y() * length , xAxis.z() * length);
		glVertex3f(0,0,0);
		glEnd();

		glColor4f(0,1,0,1);
		glBegin(GL_LINES);
		glVertex3f(yAxis.x() * length , yAxis.y() * length , yAxis.z() * length);
		glVertex3f(0,0,0);
		glEnd();

		glColor4f(0,0,1,1);
		glBegin(GL_LINES);
		glVertex3f(zAxis.x() * length , zAxis.y() * length ,zAxis.z() * length);
		glVertex3f(0,0,0);
		glEnd();
		glPopMatrix();*/

		glRotatef(angle.z(), 0, 0, 1); 
		glRotatef(angle.y(), 0, 1, 0);
		glRotatef(angle.x(), 1, 0, 0);
		drawAxis(length);
		glPopMatrix();
	}
	void drawWireFrame(){}
	void releaseResource(){}


protected:
	void keyPressEvent(QKeyEvent *e)
	{
		if (e->key() == Qt::Key_F5)
		{
			sk->resetValues();
		}
		else if (e->key() == Qt::Key_F6)
		{
		}
		else if(e->key() == Qt::Key_F9)
		{
			std::cout<<"widget size:"<<this->size().width()<<this->size().height()<<std::endl;
		}
		else if(e->key() == Qt::Key_F1)
		{
			/*Etoile::Quaternionf q = _manipulator.getCurrentRotation();
			Etoile::Joint* j = _skeleton->getJoint(_selectedJointIndex);

			_pSolver = new Etoile::JacobianDLSSolver();
			_pSolver->compute(_skeleton->getJoints(), Etoile::Vec3f(_manipulator.getPosition().x, _manipulator.getPosition().y, _manipulator.getPosition().z), false);*/

		}
		else if(e->key() == Qt::Key_F2)
		{
			//body->update(.10f);
		}
		else if(e->key() == Qt::Key_F3)
		{

			//body->addForce(Eigen::Vector3f(-10,0,0));

		}else if(e->key() == Qt::Key_F4)
		{
			//body->addTorque(Eigen::Vector3f(0,1,0));
		}else if(e->key() == Qt::Key_Up)
		{

		}else if(e->key() == Qt::Key_Down)
		{

		}
		else if(e->key() == Qt::Key_Space)
		{
		}
		else
			QGLViewer::keyPressEvent(e);
	}

	void mouseMoveEvent(QMouseEvent* event)
	{
		if ( (event->modifiers() & Qt::ShiftModifier) )
		{
		}
		else
			QGLViewer::mouseMoveEvent(event);
	}
	void mouseDoubleClickEvent (QMouseEvent * event)
	{
		float minScreenDistance = 10000;
		if(sk!=NULL)
		{
			// select one mass
			qglviewer::Camera *cam = this->camera();
			int x = event->x();
			int y = event->y();
			

			int i = 0;
			std::string name;
			Vector3_ posM;
			const std::vector<Skeleton::Joint>& joints = sk->m_joints;

			for(int i = 0; i < joints.size(); ++i)
			{
				Vector3_ pos = sk->m_joint_globalPositions[i];
				qglviewer::Vec screenPos = cam->projectedCoordinatesOf(qglviewer::Vec(pos[0],pos[1],pos[2]));
				double dis = Etoile::Vec2d(   Etoile::Vec2d(screenPos[0] , screenPos[1]) - Etoile::Vec2d(x , y)   ).length();
				if( dis < minScreenDistance )
				{
					minScreenDistance = dis;
					_selectedJointIndex = i;
					name = sk->m_joints[i].m_name;
					posM = pos;
				}
			}
			std::cout<<"selected Joint: " << name<<std::endl;
			if(sk)
			{
				for(int i = 0; i < sk->m_endeffectors.size(); ++i)
				{
					if(_selectedJointIndex == sk->m_endeffectors[i])
					{
						_manipulator.setOrigine(qglviewer::Vec(posM.x(), posM.y(), posM.z()));
					}
				}
			}else{
				/*if(_selectedJointIndex == joints.size() - 1){
				_manipulator.reset();
				_manipulator.setOrigine(qglviewer::Vec(posM.x(), posM.y(), posM.z()));
				}*/
			}
		}
		//if(minScreenDistance > 1)
			//QGLViewer::mouseDoubleClickEvent(event);
	}

	void mousePressEvent(QMouseEvent* event)
	{
		if ((event->button() == Qt::MiddleButton))
		{
			QMenu menu( this );
			std::map<QAction*, int> menuMap;
			menuMap[menu.addAction("JacobianCov")] = 1;
			menuMap[menu.addAction("JacobianDLSSolver")] = 2;
			menuMap[menu.addAction("PseudoInverse")] = 3;
			menuMap[menu.addAction("Transpose")] = 4;
			menuMap[menu.addAction("GaussianProcess")] = 5;
			//menuMap[menu.addAction("DLS")] = 5;

			menu.setMouseTracking(true);
			QAction* action = menu.exec(event->globalPos());
			if(action!= NULL)
			{
				int id = (menuMap.find(action)->second);
				if(id == 1)
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new JacobianCov(m_parameter.max_iterations, m_parameter.distance_threshold, m_parameter.damping1, m_parameter.damping2);
				}
				else if(id == 2)
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new JacobianDLSSolver(m_parameter.max_iterations, m_parameter.distance_threshold, m_parameter.damping1);
				}
				else if(id == 3)
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new JacobianPseudoInverse(m_parameter.max_iterations, m_parameter.distance_threshold, m_parameter.damping1);
				}
				else if(id == 4)
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new JacobianTranspose(m_parameter.max_iterations, m_parameter.distance_threshold, m_parameter.damping1);
				}
				else if(id == 5)
				{
					if(sk==NULL) return;
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new GPIKsolver(sk);
				}
				_pSolver->setRootActive(m_parameter.usingroot);
				//else if(id == 5)
				//{
				//	if(_pSolver != NULL) delete _pSolver;
				//	_pSolver = new Etoile::JacobianDLSSolver(maxnb_iterations);
				//}else
				//{
				//	if(_pSolver != NULL) delete _pSolver;
				//	//_pSolver = new Etoile::MassSpringSolver(maxnb_iterations);
				//}
				//std::cout<<"now using : " <<_pSolver->getIKSolverName() <<std::endl;
			}
		}
		else if ((event->modifiers() /*& Qt::ShiftModifier*/))
		{



		}
		else
		{

			QGLViewer::mousePressEvent(event);
		}
	}

	void mouseReleaseEvent(QMouseEvent* event)
	{
		QGLViewer::mouseReleaseEvent(event);
	}

	void wheelEvent ( QWheelEvent * event )
	{
		if (event->modifiers() & Qt::ShiftModifier)
		{

		}
		else
			QGLViewer::wheelEvent(event);
	}

private:
	virtual void draw()
	{
		if(animationframes.size() > 0 && sk != NULL)
		{
			sk->m_dim_values = animationframes[0].m_values;
			animationframes.erase(animationframes.begin());
			sk->update();
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		printOpenGLError();



		GLfloat light_position0[] = { 0, 30, 50, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

		GLfloat light_position1[] = { 0, 30, -50, 1.0 };
		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

		printOpenGLError();

		if(sk != NULL)
			drawIK();
		//drawBox();
		drawPlane();
		if(sk != NULL)
			drawInfo(_selectedJointIndex);

		printOpenGLError();
		//drawAxis();
		glColor3f(0.9,0.25,0.55);
		QFont serifFont("Times", 10, QFont::Bold);
		drawText((int)30, (int)25, QString(" double click : choose one joint !"), serifFont);

		glPushMatrix();
		_manipulator.draw();
		glPopMatrix();

		if(_pSolver != NULL)
		{
			drawText((int)600, (int)25, QString(_pSolver->getIKSolverName().c_str()), serifFont);
			drawText((int)600, (int)40, QString("Max Iterations: %1").arg(_pSolver->getMaxNumberOfTries()), serifFont);
			drawText((int)600, (int)55, QString("Distance Threshold: %1").arg(_pSolver->getTargetThreshold()), serifFont);

			drawText((int)600, (int)70, QString("solve Time: %1 msc").arg(speed), serifFont);
		}
	}

	virtual void init()
	{
		setMouseTracking(true);
		setFocusPolicy(Qt::StrongFocus);
		setShortcut(DISPLAY_FPS,Qt::CTRL+Qt::Key_F);

		//this->setFixedSize(WIDGETWIDTH, WIDGETHEIGHT );
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT,GL_FILL);
		//glDisable(GL_COLOR_MATERIAL);
		glClearColor(0.8, 0.8, 0.8, 0.99);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_MULTISAMPLE);

		qglviewer::Camera *cam = this->camera();
		cam->setSceneCenter(qglviewer::Vec(0,0,0));
		cam->setSceneRadius(5);
		cam->setPosition(qglviewer::Vec(0, 0.5,2));

		this->setMouseTracking(true);

		setHandlerKeyboardModifiers(QGLViewer::FRAME, Qt::AltModifier);
		setHandlerKeyboardModifiers(QGLViewer::CAMERA, Qt::NoModifier);
		setHandlerKeyboardModifiers(QGLViewer::FRAME, Qt::ControlModifier);

		setAnimationPeriod(40);
		startAnimation();

		GLfloat light_ambient0[] = { 0.5, 0.5, 0.5, 1.0 };
		GLfloat light_diffuse0[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_specular0[] = { 1.0, 1.0, 1.0, 1.0 };

		glEnable(GL_LIGHTING);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);

		GLfloat light_ambient1[] = { 0.5, 0.5, 0.5, 1.0 };
		GLfloat light_diffuse1[] = { 1.0, 1.0, 1.0, 1.0 };
		GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 1.0 };

		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
		glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);

		connect(&_manipulator, SIGNAL(moved()), this, SLOT(applyJoint()));
		//_pSolver = new Etoile::JacobianDLSSolver();
	}

	/*virtual QSize sizeHint () const
	{
	return QSize(WIDGETWIDTH,WIDGETHEIGHT);
	}
	virtual QSize minimumSizeHint () const
	{
	return QSize(WIDGETWIDTH,WIDGETHEIGHT);
	}
	virtual QSize maxmumSizeHint () const
	{
	return QSize(WIDGETWIDTH,WIDGETHEIGHT);
	}*/

	void drawIK()
	{
		glMatrixMode(GL_MODELVIEW);
		for(int i = 0; i < sk->m_joints.size(); ++i)
		{
			Matrix3_ matrix = sk->m_joint_globalOrientations[i];
			Vector3_ pos = sk->m_joint_globalPositions[i];
			float mat[4][4];
			for(int w = 0; w < 3; ++w)
				for(int h = 0; h < 3; ++h)
				{
					mat[w][h] = matrix(h,w);
				}
				mat[3][0] = 0;
				mat[3][1] = 0;
				mat[3][2] = 0;
				mat[3][3] = 1;
				mat[0][3] = 0;
				mat[1][3] = 0;
				mat[2][3] = 0;

				glPushMatrix();
				//glLoadIdentity();
				glTranslatef(pos(0),pos(1),pos(2));
				glPushMatrix();
				glMultMatrixf(&mat[0][0]);
				QGLViewer::drawAxis(0.05); // Or any scale
				glBegin( GL_POINTS);
				glVertex3f(0,0,0);
				glEnd();
				glPopMatrix();
				glPopMatrix();

				glColor3f(0.1,0.4,0.4);
				Etoile::drawSphere_convenient(pos.x(), pos.y(), pos.z(), 0.03, 10, 10);
				int parent = sk->m_joints[i].m_index_parent;
				if(parent >= 0)
				{
					glColor3f(0.1,0.4,0.7);
					Etoile::drawCylinder_convenient(pos.x(), pos.y(), pos.z(), sk->m_joint_globalPositions[parent].x(), sk->m_joint_globalPositions[parent].y(), sk->m_joint_globalPositions[parent].z() , 0.02, 10);
				}
		}
	}

	QMainWindow* _pParent;

signals:

	public slots:
		void applyJoint()
		{
			//sk->m_dim_values[6] = 1;
			//sk->m_dim_values[11] = 1;
			//sk->update();

			if(sk==NULL) return;
			if(_pSolver != NULL)
			{

				std::vector<Vector3_> targets;
				VectorX_ t = VectorX_::Zero(sk->m_endeffectors.size() * 3);
				for(unsigned int i = 0 ; i < sk->m_endeffectors.size(); ++i)
				{
					if(sk->m_endeffectors[i] == _selectedJointIndex)
					{
						targets.push_back(Vector3_(_manipulator.getPosition().x, _manipulator.getPosition().y, _manipulator.getPosition().z));	
					}
					else
					{
						targets.push_back(sk->m_joint_globalPositions[sk->m_endeffectors[i]]);
					}
					Matrix3_ orientation =sk->m_joint_globalOrientations[0];
					Vector3_ pos = sk->m_joint_globalPositions[0];
					Vector3_ cur = orientation.transpose() * (targets[i] - pos);
					t(i * 3) = cur(0);
					t(i * 3 + 1) = cur(1);
					t(i * 3 + 2) = cur(2);
				}
				JacobianCov* sol = dynamic_cast<JacobianCov*>(_pSolver);
				if(sol != NULL)
				{
					TargetGaussian tg = m_gp.computeASample(t);
					sol->setParameters(tg.m_invcov, tg.m_mu);
					if(_frameIdx >= 0)
					{
						//sk->m_dim_values = pos;
						//std::cout<<pos.size()<<std::endl;
						sk->update();
					}else
					{
						//sk->resetValues();
					}
				}

				GPIKsolver* sol2 = dynamic_cast<GPIKsolver*>(_pSolver);
				if(sol2 != NULL)
				{
					sol2->computeASample(t);
				}

				JacobianDLSSolver* sol3 = dynamic_cast<JacobianDLSSolver*>(_pSolver);
				if(sol != NULL)
				{
					sk->resetValues();
				}

				QElapsedTimer timer;
				timer.start();
				_pSolver->solve(sk, targets);
				int nano = timer.nsecsElapsed();
				speed = nano * 0.000001;
			}


		}

public:

	int _selectedJointIndex;
	bool _useConstraints;
	int _frameIdx;

	unsigned int _fps;
	double _scaleFactor;
	std::vector<double> pos;
	Etoile::glDrawFunctions _glDrawFunctions;

	Etoile::SimpleManipulator _manipulator;
	IKSolver* _pSolver;
	Skeleton* sk;
	int number_bones;
	Etoile::Vec3f _original;
	double speed;
	std::vector<Frame> animationframes;

	GaussianProcess m_gp;
	SolverParamter m_parameter;
};
