#ifndef SCENETREEWIDGET_H
#define SCENETREEWIDGET_H

#include <QTreeView>
#include "ui_SceneTreeWidget.h"

class SceneTreeWidget : public QTreeView
{
	Q_OBJECT

public:
	SceneTreeWidget(QWidget *parent = 0);
	~SceneTreeWidget();

private:
	Ui::SceneTreeWidget ui;
};

#endif // SCENETREEWIDGET_H
