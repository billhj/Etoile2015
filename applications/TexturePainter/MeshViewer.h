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

using namespace Etoile;

class MeshViewer : public QGLRenderWidget
{
	Q_OBJECT

public:
	enum Mode{VIEW_MODE, PAINT_TEXTURE_MODE, GEOMETRY_MODE};
public:
	MeshViewer(QWidget *parent = 0);
	~MeshViewer();
	void init();
	void draw();
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent* const event) override;
	virtual void mousePressEvent(QMouseEvent* const event) override;
	virtual void mouseReleaseEvent(QMouseEvent* const event) override;
	virtual void mouseMoveEvent(QMouseEvent* const event) override;
	virtual void wheelEvent(QWheelEvent* const event) override;
	virtual void keyPressEvent(QKeyEvent* const event) override;

private:
	int m_x, m_y;
	bool m_picked;
	int m_mode;
};