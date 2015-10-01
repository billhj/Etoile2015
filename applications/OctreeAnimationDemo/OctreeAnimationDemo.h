#ifndef OCTREEANIMATIONDEMO_H
#define OCTREEANIMATIONDEMO_H

#include <QtGui/QMainWindow>
#include "ui_OctreeAnimationDemo.h"

class OctreeAnimationDemo : public QMainWindow
{
	Q_OBJECT

public:
	OctreeAnimationDemo(QWidget *parent = 0, Qt::WFlags flags = 0);
	~OctreeAnimationDemo();

private:
	Ui::OctreeAnimationDemoClass ui;
};

#endif // OCTREEANIMATIONDEMO_H
