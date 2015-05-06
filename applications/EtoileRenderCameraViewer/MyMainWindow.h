#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QMainWindow>
#include "ui_MyMainWindow.h"
#include "MeshViewer.h"
class MyMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MyMainWindow(QWidget *parent = 0);
	~MyMainWindow();

private:
	Ui::MyMainWindow ui;
	MeshViewer* _pWidget;
};

#endif // MYMAINWINDOW_H
