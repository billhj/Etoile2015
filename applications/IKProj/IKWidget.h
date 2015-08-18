/**
* Copyright(C) 2009-2012                
* @author Jing HUANG 
* @file IKWidget.h
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
#include "geometry/AxisAlignedBoundingBox.h"
#include "Manipulator.h"
#include "animation/IK/JacobianDLSSolver.h"
#include "animation/IK/JacobianDLSSVDSolver.h"
#include "animation/IK/JacobianRcompensedDLSSolver.h"
#include "animation/IK/JacobianPseudoInverseSolver.h"
#include "animation/IK/JacobianTransposeSolver.h"
#include <QTime>
#include "renderer/OpenGL/glfunctions.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif


class IKWidget : public QGLViewer
{
	Q_OBJECT

		static const int WIDGETWIDTH = 800, WIDGETHEIGHT = 600;
public:
	int framesNb;
	IKWidget(QMainWindow* parent = 0): _pParent(parent), _scaleFactor(0.2), _selectedJointIndex(-1)
	{
		framesNb = 0;
		chain = new Etoile::IKTree();
		
		Etoile::IKTree* tree = dynamic_cast<Etoile::IKTree*>(chain);
		if(tree)
		{
			tree->loadFromFile("simpleTree.sk");
			for(int i = 0; i < tree->m_joint_end_effector_index.size(); ++i)
			{
				targets.push_back(tree->m_joint_globalPositions[tree->m_joint_end_effector_index[i]]);
			}
		}else
		{
			chain->loadFromFile("simple.sk");
		}
		targetId = 0;
	}

	~IKWidget(){}

	

	void drawInfo(int currentJoint)
	{
		if(currentJoint < 0) return;

		Etoile::Matrix3_ matrix = chain->m_joint_globalOrientations[currentJoint];
		
		Etoile::Vector3_ pos = chain->m_joint_globalPositions[currentJoint];
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
		drawText((int)screenPos[0]+20, (int)screenPos[1]-10, QString(" %4: [ %1, %2, %3 ] ").arg(pos.x(), 0, 'f',2).arg(pos.y(), 0, 'f',2).arg(pos.z(), 0, 'f',2).arg(QString(chain->m_joints[currentJoint]->m_name.c_str())) );

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
		drawGrid(5.0,20);		
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

	void setIKSolver(Etoile::IKSolver* solver)
	{
		_pSolver = solver;
	}

	Etoile::IKSolver* getIKSolver(){return _pSolver;}

protected:
	void keyPressEvent(QKeyEvent *e)
	{
		if (e->key() == Qt::Key_F5)
		{
			
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

	void mousePressEvent(QMouseEvent* event)
	{
		if ((event->button() == Qt::MiddleButton))
		{
			QMenu menu( this );
			std::map<QAction*, int> menuMap;
			menuMap[menu.addAction("JacobianRcompensedDLSSolver")] = 1;
			menuMap[menu.addAction("JacobianDLSSVDSolver")] = 2;
			menuMap[menu.addAction("PseudoInverse")] = 3;
			menuMap[menu.addAction("Transpose")] = 4;
			menuMap[menu.addAction("DLS")] = 5;

			menu.setMouseTracking(true);
			QAction* action = menu.exec(event->globalPos());
			if(action!= NULL)
			{
				int id = (menuMap.find(action)->second);
				int maxnb_iterations = _pSolver->getMaxNumberOfTries();
				if(id == 1)
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new Etoile::JacobianRcompensedDLSSolver(maxnb_iterations);
				}
				else if(id == 2)
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new Etoile::JacobianDLSSVDSolver(maxnb_iterations);
				}
				else if(id == 3)
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new Etoile::JacobianPseudoInverseSolver(maxnb_iterations);
				}
				else if(id == 4)
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new Etoile::JacobianTransposeSolver(maxnb_iterations);
				}
				else if(id == 5)
				{
					if(_pSolver != NULL) delete _pSolver;
					_pSolver = new Etoile::JacobianDLSSolver(maxnb_iterations);
				}else
				{
					if(_pSolver != NULL) delete _pSolver;
					//_pSolver = new Etoile::MassSpringSolver(maxnb_iterations);
				}
				std::cout<<"now using : " <<_pSolver->getIKSolverName() <<std::endl;
			}
		}
		else if ((event->modifiers() & Qt::ShiftModifier))
		{

			// select one mass
			qglviewer::Camera *cam = this->camera();
			int x = event->x();
			int y = event->y();
			float minScreenDistance = 10000;

			int i = 0;
			std::string name;
			Etoile::Vector3_ posM;
			const std::vector<Etoile::IKChain::Joint*>& joints = chain->m_joints;
			
			for(int i = 0; i < joints.size(); ++i)
			{
				Etoile::Vector3_ pos = chain->m_joint_globalPositions[i];
				qglviewer::Vec screenPos = cam->projectedCoordinatesOf(qglviewer::Vec(pos[0],pos[1],pos[2]));
				double dis = Etoile::Vec2d(   Etoile::Vec2d(screenPos[0] , screenPos[1]) - Etoile::Vec2d(x , y)   ).length();
				if( dis < minScreenDistance )
				{
					minScreenDistance = dis;
					_selectedJointIndex = i;
					name = chain->m_joints[i]->m_name;
					posM = pos;
				}
			}
			std::cout<<"selected Joint: " << name<<std::endl;
			Etoile::IKTree* tree = dynamic_cast<Etoile::IKTree*>(chain);
			if(tree)
			{
				for(int i = 0; i < tree->m_joint_end_effector_index.size(); ++i)
				{
					if(_selectedJointIndex == tree->m_joint_end_effector_index[i])
					{
						_manipulator.reset();
						_manipulator.setOrigine(qglviewer::Vec(posM.x(), posM.y(), posM.z()));
						targetId = i;
					}
				}
			}else{
				if(_selectedJointIndex == joints.size() - 1){
					_manipulator.reset();
					_manipulator.setOrigine(qglviewer::Vec(posM.x(), posM.y(), posM.z()));
				}
			}

		}
		else
			QGLViewer::mousePressEvent(event);
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
		
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		printOpenGLError();

		

		GLfloat light_position0[] = { 0, 30, 50, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

		GLfloat light_position1[] = { 0, 30, -50, 1.0 };
		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

		printOpenGLError();

		drawIK();
		//drawBox();
		drawPlane();
		drawInfo(_selectedJointIndex);
		
		printOpenGLError();
		//drawAxis();
		glColor3f(0.9,0.25,0.55);
		QFont serifFont("Times", 10, QFont::Bold);
		drawText((int)30, (int)25, QString(" shift + right_mouse : choose one joint !"), serifFont);

		glPushMatrix();
		_manipulator.draw();
		glPopMatrix();

		if(_pSolver != NULL)
		{
			drawText((int)600, (int)25, QString(_pSolver->getIKSolverName().c_str()), serifFont);
			drawText((int)600, (int)40, QString("Max Iterations: %1").arg(_pSolver->getMaxNumberOfTries()), serifFont);
			drawText((int)600, (int)55, QString("Distance Threshold: %1").arg(_pSolver->getTargetThreshold()), serifFont);
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
		cam->setPosition(qglviewer::Vec(0,0.5,1));

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
		_pSolver = new Etoile::JacobianDLSSolver();
	}

	virtual QSize sizeHint () const
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
	}

	void drawIK()
	{
		glMatrixMode(GL_MODELVIEW);
		for(int i = 0; i < chain->m_joints.size(); ++i)
		{
			Etoile::Matrix3_ matrix = chain->m_joint_globalOrientations[i];
			Etoile::Vector3_ pos = chain->m_joint_globalPositions[i];
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
			Etoile::drawSphere_convenient(pos.x(), pos.y(), pos.z(), 0.02, 10, 10);
			int parent = chain->m_joints[i]->m_index_parent;
			if(parent >= 0)
			{
				glColor3f(0.1,0.4,0.7);
				Etoile::drawCylinder_convenient(pos.x(), pos.y(), pos.z(), chain->m_joint_globalPositions[parent].x(), chain->m_joint_globalPositions[parent].y(), chain->m_joint_globalPositions[parent].z() , 0.01, 10);
			}
		}
	}

	QMainWindow* _pParent;

signals:

	public slots:
		void applyJoint()
		{
			QTime myTimer;
			myTimer.start();
			if(_pSolver != NULL)
			{
				Etoile::IKTree* tree = dynamic_cast<Etoile::IKTree*>(chain);
				if(tree)
				{
					targets[targetId] = Etoile::Vector3_(_manipulator.getPosition().x, _manipulator.getPosition().y, _manipulator.getPosition().z);
					_pSolver->solve(tree, targets, true);
				}else
					_pSolver->solve(chain, Etoile::Vector3_(_manipulator.getPosition().x, _manipulator.getPosition().y, _manipulator.getPosition().z), true);
				//int nMilliseconds = myTimer.elapsed();
				//std::cout<<_pSolver->getIKSolverName() << " nMilliseconds : " << nMilliseconds<<std::endl;
			}
				
		}
public:

	int _selectedJointIndex;
	bool _useConstraints;
	int _frameIdx;

	unsigned int _fps;
	Etoile::AxisAlignedBoundingBoxf _boundingBox;
	double _scaleFactor;

	Etoile::glDrawFunctions _glDrawFunctions;

	Etoile::SimpleManipulator _manipulator;
	Etoile::IKSolver* _pSolver;
	Etoile::IKChain* chain;
	int number_bones;
	Etoile::Vec3f _original;
	std::vector<Etoile::Vector3_> targets;
	int targetId;
	//std::vector<IKSolver*> _solvers;
};
