#ifndef OCTREESKELETONWIDGET_H
#define OCTREESKELETONWIDGET_H

#include <QWidget>
#include "ui_OctreeSkeletonWidget.h"
#include "OctreeSkeleton.h"

class OctreeSkeletonWidget : public QWidget
{
	Q_OBJECT

public:
	OctreeSkeletonWidget(QWidget *parent = 0);
	~OctreeSkeletonWidget();

private:
	Ui::OctreeSkeletonWidget ui;
	OctreeSkeleton *octreeskeleton;
};

#endif // OCTREESKELETONWIDGET_H
