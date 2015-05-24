#ifndef SCENETREEWIDGET_H
#define SCENETREEWIDGET_H

#include "ui_QTSceneTreeWidget.h"

class QTSceneTreeWidget : public QWidget
{
	Q_OBJECT

public:
	QTSceneTreeWidget(QWidget *parent = 0);
	~QTSceneTreeWidget();

private:
	Ui::QTSceneTreeWidget ui;
};

#endif // SCENETREEWIDGET_H
