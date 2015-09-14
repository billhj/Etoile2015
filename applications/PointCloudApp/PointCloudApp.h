#ifndef POINTCLOUDAPP_H
#define POINTCLOUDAPP_H

#include <QtGui/QMainWindow>
#include "ui_PointCloudApp.h"
#include "MeshViewer.h"

class PointCloudApp : public QMainWindow
{
	Q_OBJECT

public:
	PointCloudApp(QWidget *parent = 0, Qt::WFlags flags = 0);
	~PointCloudApp();

private:
	Ui::PointCloudAppClass ui;
		MeshViewer* _pWidget;
};

#endif // POINTCLOUDAPP_H
