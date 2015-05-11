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
	MeshViewer(QWidget *parent = 0);
	~MeshViewer();
	void init();
	void draw();
protected:
	void mouseDoubleClickEvent(QMouseEvent* const event);
private:
	int m_x, m_y;
	bool m_picked;
};