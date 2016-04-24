#ifndef BVHAPP_H
#define BVHAPP_H

#include <QtGui/QMainWindow>
#include "ui_BVHApp.h"

class BVHApp : public QMainWindow
{
	Q_OBJECT

public:
	BVHApp(QWidget *parent = 0, Qt::WFlags flags = 0);
	~BVHApp();

private:
	Ui::BVHAppClass ui;
};

#endif // BVHAPP_H
