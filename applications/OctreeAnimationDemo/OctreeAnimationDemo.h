#ifndef OCTREEANIMATIONDEMO_H
#define OCTREEANIMATIONDEMO_H

#include <QtGui/QMainWindow>
#include "ui_OctreeAnimationDemo.h"
#include "OctreeAnimation.h"

class OctreeAnimationDemo : public QMainWindow
{
	Q_OBJECT

public:
	OctreeAnimationDemo(QWidget *parent = 0, Qt::WFlags flags = 0);
	~OctreeAnimationDemo();

	public slots:
		void loadFile();
		void saveFile();
		void appendFile();
		void saveTXT();
		void runBVH();
private:
	Ui::OctreeAnimationDemoClass ui;
	OctreeAnimation* p_anim;
};

#endif // OCTREEANIMATIONDEMO_H
