#ifndef QGLRENDERWIDGET_H
#define QGLRENDERWIDGET_H

#include <QGLWidget>
#include <QTime>
#include <QTimer>
#include "ui_QGLRenderWidget.h"

namespace Etoile
{
	class Camera;
	class QGLRenderWidget : public QGLWidget
	{
		Q_OBJECT

	public:
		QGLRenderWidget(QWidget *parent = 0);
		~QGLRenderWidget();
		void defaultSetup();
		QColor backgroundColor() const { return m_backgroundColor; }
		QColor foregroundColor() const { return m_foregroundColor; }
		bool FPSIsDisplayed() const { return m_FPSIsDisplayed; }
		void drawText(int x, int y, const QString& text, const QFont& fnt = QFont());
		qreal currentFPS() { return m_f_p_s; }
		bool isFullScreen() const { return m_fullScreen; }
		bool animationIsStarted() const { return m_animationStarted; }
		int animationPeriod() const { return m_animationPeriod; }
		Camera* camera(){ return p_camera; }


		void loadProjectionMatrix(bool reset = false);
		void loadModelViewMatrix(bool reset = false);
		virtual void animate();
	protected:
		void initializeGL();
		void paintGL();
		void resizeGL(int width, int height);

		virtual void init() = 0;
		virtual void draw() = 0;
		virtual void preDraw();
		virtual void fastDraw();
		virtual void postDraw();


		virtual void mousePressEvent(QMouseEvent* const event) override;
		virtual void mouseDoubleClickEvent(QMouseEvent* const event) override;
		/*! Mouse release event callback method. See mousePressEvent(). */
		virtual void mouseReleaseEvent(QMouseEvent* const event) override;
		virtual void mouseMoveEvent(QMouseEvent* const event) override;
		virtual void wheelEvent(QWheelEvent* const event) override;
		virtual void timerEvent(QTimerEvent *);
		virtual void keyPressEvent(QKeyEvent *);
		virtual void closeEvent(QCloseEvent *);

		public Q_SLOTS:
		/*! Sets the backgroundColor() of the viewer and calls \c qglClearColor(). See also
		setForegroundColor(). */
		void setBackgroundColor(const QColor& color) { m_backgroundColor = color; qglClearColor(color); }
		/*! Sets the foregroundColor() of the viewer, used to draw visual hints. See also setBackgroundColor(). */
		void setForegroundColor(const QColor& color) { m_foregroundColor = color; }

		void displayFPS();
		void setFullScreen(bool fullScreen = true);
		void setAnimationPeriod(int period) { m_animationPeriod = period; }
		virtual void startAnimation();
		virtual void stopAnimation();
		void setDrawAxis(bool draw){m_drawAxis = draw;}
		void setDrawGrid(bool draw){m_drawGrid = draw;}
		void showAxis(){m_drawAxis = !m_drawAxis;}
		void showGrid(){m_drawGrid = !m_drawGrid;}
	public:
	Q_SIGNALS :
		/*! Signal emitted by the default init() method.
		Connect this signal to the methods that need to be called to initialize your viewer or overload init(). */
		void viewerInitialized();
			  /*! Signal emitted by the default draw() method.
			  Connect this signal to your main drawing method or overload draw(). See the <a
			  href="../examples/callback.html">callback example</a> for an illustration. */
			  void drawNeeded();
			  /*! Signal emitted at the end of the QGLViewer::paintGL() method, when frame is drawn.
			  connect(viewer, SIGNAL(drawFinished(bool)), SLOT(saveSnapshot(bool)));*/
			  void drawFinished(bool automatic);

	protected:
		Ui::QGLRenderWidget ui;
		QColor m_backgroundColor, m_foregroundColor;
		bool m_fullScreen;
		bool m_FPSIsDisplayed;
		bool m_textIsEnabled;	// drawText() actually draws text or not

		QTime m_fpsTime;
		unsigned int m_fpsCounter;
		QString m_fpsString;
		qreal m_f_p_s;

		QString m_message;
		bool m_displayMessage;

		bool m_drawGrid;
		bool m_drawAxis;

		bool m_animationStarted; // animation mode started
		int m_animationPeriod;
		int m_animationTimerId;
		Camera* p_camera;	
	};
}
#endif // QGLRENDERWIDGET_H
