/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file EtoileMeshViewer.h
* @brief 
* @date 1/2/2011
*/

#include <QtGui/QMainWindow>
#include "ui_EtoileMeshViewer.h"
#include "MeshViewer.h"
#include "CameraWidget.h"

class EtoileMeshViewer : public QMainWindow
{
	Q_OBJECT

public:
	EtoileMeshViewer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~EtoileMeshViewer();
	void addMenu();
public slots:
	void openOBJ();
	void openAbout();
private:
	Ui::EtoileMeshViewerClass ui;
	MeshViewer* _pWidget;
	CameraWidget* _pCameraWidget;
};

