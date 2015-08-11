#ifndef IKPROJ_H
#define IKPROJ_H

#include <QtGui/QMainWindow>
#include "ui_IKProj.h"

class IKProj : public QMainWindow
{
	Q_OBJECT

public:
	IKProj(QWidget *parent = 0, Qt::WFlags flags = 0);
	~IKProj();

private:
	Ui::IKProjClass ui;
};

#endif // IKPROJ_H
