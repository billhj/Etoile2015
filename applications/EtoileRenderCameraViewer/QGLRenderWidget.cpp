#include "QGLRenderWidget.h"
#include <QTimer>
#include <QMouseEvent>
#include "geometry/Camera.h"
#include <iostream>
#include "geometry/TrackingBallCameraManipulator.h"

namespace Etoile
{
	TrackingBallCameraManipulator* manipulator;
	QGLRenderWidget::QGLRenderWidget(QWidget *parent)
		: QGLWidget(parent)
	{
		ui.setupUi(this);
		defaultSetup();
	}

	QGLRenderWidget::~QGLRenderWidget()
	{

	}

	void QGLRenderWidget::defaultSetup(){
		setFocusPolicy(Qt::StrongFocus);
		p_camera = new Camera(Vec3f(0,0,0), Vec3f(0,1,0), Vec3f(0,0,2));
		manipulator = new TrackingBallCameraManipulator();
		p_camera->setManipulator(manipulator);

		m_fpsTime.start();
		m_fpsCounter = 0;
		m_f_p_s = 0.0;
		m_fpsString = tr("%1Hz", "Frames per seconds, in Hertz").arg("?");
		m_displayMessage = false;

		m_fullScreen = false;
		setFullScreen(false);

		m_animationTimerId = 0;
		//stopAnimation();
		setAnimationPeriod(5); // 25Hz
		startAnimation();

		setAttribute(Qt::WA_NoSystemBackground);

	}


	void QGLRenderWidget::initializeGL()
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_MATERIAL);

		// Default colors
		setForegroundColor(QColor(180, 180, 180));
		setBackgroundColor(QColor(51, 51, 51));

		// Clear the buffer where we're going to draw
		if (format().stereo())
		{
			glDrawBuffer(GL_BACK_RIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawBuffer(GL_BACK_LEFT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calls user defined method. Default emits a signal.
		init();

		// Give time to glInit to finish and then call setFullScreen().
		if (isFullScreen())
			QTimer::singleShot(100, this, SLOT(delayedFullScreen()));
	}

	void QGLRenderWidget::paintGL()
	{
		//if (displaysInStereo())
		//{
		//	for (int view = 1; view >= 0; --view)
		//	{
		//		// Clears screen, set model view matrix with shifted matrix for ith buffer
		//		//preDrawStereo(view);
		//		
		//		draw();
		//		postDraw();
		//	}
		//}
		//else
		//{
			// Clears screen, set model view matrix...
			preDraw();
			draw();
			// Add visual hints: axis, camera, grid...
			postDraw();
		//}
		Q_EMIT drawFinished(true);
	}


	/*! Sets OpenGL state before draw().

	Default behavior clears screen and sets the projection and modelView matrices:
	\code
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera()->loadProjectionMatrix();
	camera()->loadModelViewMatrix();
	\endcode

	Emits the drawNeeded() signal once this is done (see the <a href="../examples/callback.html">callback example</a>). */
	void QGLRenderWidget::preDraw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// GL_PROJECTION matrix
		loadProjectionMatrix(true);
		// GL_MODELVIEW matrix
		loadModelViewMatrix(true);

		Q_EMIT drawNeeded();
	}

	void QGLRenderWidget::loadProjectionMatrix(bool reset)
	{
		
		glMatrixMode(GL_PROJECTION);
		p_camera->computeProjectionMatrix();
		float* projectM = p_camera->getGLProjectionMatrix();
		if (reset)
		{
			glLoadIdentity();
		}
		glMultMatrixf(projectM);
	}

	void QGLRenderWidget::loadModelViewMatrix(bool reset)
	{
		glMatrixMode(GL_MODELVIEW);
		p_camera->computeModelViewMatrix();
		float* modelVM = p_camera->getGLModelViewMatrix();
		if (reset)
		{
			glLoadIdentity();
		}
		glMultMatrixf(modelVM);
	}


	/*! Called after draw() to draw viewer visual hints.

	Default implementation displays axis, grid, FPS... when the respective flags are sets.

	See the <a href="../examples/multiSelect.html">multiSelect</a> and <a
	href="../examples/contribs.html#thumbnail">thumbnail</a> examples for an overloading illustration.

	The GLContext (color, LIGHTING, BLEND...) is \e not modified by this method, so that in
	draw(), the user can rely on the OpenGL context he defined. Respect this convention (by pushing/popping the
	different attributes) if you overload this method. */
	void QGLRenderWidget::postDraw()
	{
		// Reset model view matrix to world coordinates origin
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		loadModelViewMatrix(true);
		// TODO restore model loadProjectionMatrixStereo

		// Save OpenGL state
		glPushAttrib(GL_ALL_ATTRIB_BITS);

		// Set neutral GL state
		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_2D);
		//glDisable(GL_TEXTURE_3D);

		glDisable(GL_TEXTURE_GEN_Q);
		glDisable(GL_TEXTURE_GEN_R);
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);


		glDisable(GL_COLOR_MATERIAL);
		qglColor(foregroundColor());


		// FPS computation
		const unsigned int maxCounter = 20;
		if (++m_fpsCounter == maxCounter)
		{
			m_f_p_s = 1000.0 * maxCounter / m_fpsTime.restart();
			m_fpsString = tr("%1Hz", "Frames per seconds, in Hertz").arg(m_f_p_s, 0, 'f', ((m_f_p_s < 10.0) ? 1 : 0));
			m_fpsCounter = 0;
		}

		// Restore foregroundColor
		float color[4];
		color[0] = foregroundColor().red() / 255.0f;
		color[1] = foregroundColor().green() / 255.0f;
		color[2] = foregroundColor().blue() / 255.0f;
		color[3] = 1.0;
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		if (FPSIsDisplayed()) displayFPS();
		if (m_displayMessage) drawText(10, height() - 10, m_message);

		// Restore GL state
		glPopAttrib();
		glPopMatrix();
	}


	/*! Draws a simplified version of the scene to guarantee interactive camera displacements.

	This method is called instead of draw() when the qglviewer::Camera::frame() is
	qglviewer::ManipulatedCameraFrame::isManipulated(). Default implementation simply calls draw().

	Overload this method if your scene is too complex to allow for interactive camera manipulation. See
	the <a href="../examples/fastDraw.html">fastDraw example</a> for an illustration. */
	void QGLRenderWidget::fastDraw()
	{
		draw();
	}


	void QGLRenderWidget::resizeGL(int width, int height)
	{
		QGLWidget::resizeGL(width, height);
		glViewport(0, 0, GLint(width), GLint(height));
		camera()->setWidth(width);
		camera()->setHeight(height);
	}


	void QGLRenderWidget::displayFPS()
	{
		drawText(10, int(1.5*((QApplication::font().pixelSize() > 0) ? QApplication::font().pixelSize() : QApplication::font().pointSize())), m_fpsString);

		//drawText(50, int(1.5*((QApplication::font().pixelSize() > 0) ? QApplication::font().pixelSize() : QApplication::font().pointSize())), QString().setNum(camera()->getZFarPlane()));
	}

	void QGLRenderWidget::drawText(int x, int y, const QString& text, const QFont& fnt)
	{
		if (!m_textIsEnabled)
			return;
		renderText(x, y, text, fnt);
	}

	void QGLRenderWidget::setFullScreen(bool fullScreen)
	{
		if (m_fullScreen == fullScreen) return;

		m_fullScreen = fullScreen;

		QWidget* tlw = topLevelWidget();

		/*if (isFullScreen())
		{
		prevPos_ = topLevelWidget()->pos();
		tlw->showFullScreen();
		tlw->move(0, 0);
		}
		else
		{
		tlw->showNormal();
		tlw->move(prevPos_);
		}*/
	}

	void QGLRenderWidget::timerEvent(QTimerEvent *)
	{
		if (animationIsStarted())
		{
			animate();
			updateGL();
		}
	}

	/*! Starts the animation loop. See animationIsStarted(). */
	void QGLRenderWidget::startAnimation()
	{
		m_animationTimerId = startTimer(animationPeriod());
		m_animationStarted = true;
	}

	/*! Stops animation. See animationIsStarted(). */
	void QGLRenderWidget::stopAnimation()
	{
		m_animationStarted = false;
		if (m_animationTimerId != 0)
			killTimer(m_animationTimerId);
	}

	void QGLRenderWidget::animate()
	{
		if(manipulator != NULL) manipulator->spinUpdate();
	}

	void QGLRenderWidget::mousePressEvent(QMouseEvent* const event)
	{
		if (event->button() == Qt::MouseButton::LeftButton){
			manipulator->m_move = false;
			manipulator->m_rotate = true;
			manipulator->m_zoom = false;
		}
		else if (event->button() == Qt::MouseButton::MiddleButton){
			manipulator->m_move = false;
			manipulator->m_rotate = false;
			manipulator->m_zoom = true;
		}
		else if (event->button() == Qt::MouseButton::RightButton){
			manipulator->m_move = true;
			manipulator->m_rotate = false;
			manipulator->m_zoom = false;
		}
		m_prevPos = m_pressPos = event->pos();
	}

	void QGLRenderWidget::mouseDoubleClickEvent(QMouseEvent* const event)
	{
		//p_camera->frame()->mouseDoubleClickEvent(event);
		//manipulator->setSpinActive(!manipulator->m_spin_active);
	}

	void QGLRenderWidget::mouseReleaseEvent(QMouseEvent* const event)
	{
		manipulator->m_move = false;
		manipulator->m_rotate = false;
		manipulator->m_zoom = false;
	}

	void QGLRenderWidget::mouseMoveEvent(QMouseEvent* const event)
	{
		QPoint currentPos = event->pos();
		if(manipulator->m_move)
		{
			QPoint delta = m_prevPos - event->pos();
			manipulator->moveOnScreen(delta.x(), delta.y());
		}
		else if(manipulator->m_rotate)
		{
			manipulator->rotateOnScreen(m_prevPos.x(), m_prevPos.y(), currentPos.x(), currentPos.y());
			if( (currentPos - m_prevPos).manhattanLength() > 5 )
			{
				manipulator->setSpinActive(true);
			}else
			{
				manipulator->setSpinActive(false);
			}
		}
		else if(manipulator->m_zoom)
		{
			qreal dx = qreal(event->x() - m_prevPos.x()) / p_camera->getWidth();
			qreal dy = qreal(event->y() - m_prevPos.y()) / p_camera->getHeight();
			qreal value = fabs(dx) > fabs(dy) ? dx : dy;
			manipulator->zoom(value);
		}

		m_prevPos = currentPos;
		update();
	}

	void QGLRenderWidget::wheelEvent(QWheelEvent* const event)
	{
		static const qreal WHEEL_SENSITIVITY_COEF = 8E-4;
		float value = event->delta() * manipulator->wheelSensitivity() * WHEEL_SENSITIVITY_COEF;
		manipulator->zoom(value);
		update();
	}
}