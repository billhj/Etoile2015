/**
* Copyright(C) 2009-2012                
* @author Jing HUANG
* @file BVHApp.h
* @brief 
* @date 1/2/2011
*/
#ifndef BVHAPP_H
#define BVHAPP_H

#include <QtGui/QMainWindow>
#include "ui_BVHApp.h"
#include "SkeletonWidget.h"
class BVHApp : public QMainWindow
{
	Q_OBJECT

public:
	BVHApp(QWidget *parent = 0, Qt::WFlags flags = 0);
	~BVHApp();
	void addMenu();
public slots:
	void applyIKAction(QAction*);
	void openAbout();
	void openUsage();
	void setMaxIterationsNb(int);
	void setDistanceThreshold(double);
	void openBVH();
	void playBVH();
	void accBVH();
	void deaccBVH();
	void stopBVH();
	void updateCombobox();
	void frameIndexChanged(int);
	void changeMode(bool);
	void changeDamping(double);
	void saveGenerateSequence();
	void generateSequence();
private:
	Ui::BVHAppClass ui;
	SkeletonWidget* _pIKWidget;
	bool mode;
	std::vector<Frame> _generatedFrame;
};

#endif // BVHAPP_H
