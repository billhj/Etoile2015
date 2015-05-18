/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file MeshViewer.h
* @brief 
* @date 1/2/2011
*/

#include "renderer/OpenGL/glhead.h"
#include "renderer/OpenGL/glInfo.h"
#include "QGLRenderer/QGLRenderWidget.h"
#include <QPushButton>

using namespace Etoile;

class MeshViewer : public QGLRenderWidget
{
	Q_OBJECT
	friend class TexturePainter;
public:
	enum VIEWER_MODE{VIEW_MODE, TEXTURE_MODE, GEOMETRY_MODE};
	//enum PICK_MODE{NO_MODE, TEXCOORD, COLORCOORD};
	enum TOOLS{NOTOOL, PENCIL, MOVE, PIPETTE};
public:
	MeshViewer(QWidget *parent = 0);
	~MeshViewer();
	void init();
	void draw();
	void texCoordPicking();
	void colorPicking();

	void drawOnTexture();
	public slots:
		void usePipette();
		void setViewerMode(QString mode);
		void setTool(QAbstractButton* button);

signals:
		void colorPicked();
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent* const event) override;
	virtual void mousePressEvent(QMouseEvent* const event) override;
	virtual void mouseReleaseEvent(QMouseEvent* const event) override;
	virtual void mouseMoveEvent(QMouseEvent* const event) override;
	virtual void wheelEvent(QWheelEvent* const event) override;
	virtual void keyPressEvent(QKeyEvent* const event) override;

private:
	int m_x, m_y;
	//GLfloat pixel[3];
	//PICK_MODE m_picked;
	VIEWER_MODE m_mode;
	TOOLS m_tool;
	bool m_pickOn;
	GLfloat* m_pixel;
};