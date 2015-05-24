#ifndef SCENETREEWIDGET_H
#define SCENETREEWIDGET_H

#include "ui_QTSceneTreeWidget.h"
#include "geometry/Scene.h"

namespace Etoile
{
class QTSceneTreeWidget : public QWidget
{
	Q_OBJECT

public:
	QTSceneTreeWidget(QWidget *parent = 0);
	~QTSceneTreeWidget();
	void setScene(Scene* scene);
	void updateModel();
private:
	Ui::QTSceneTreeWidget ui;
	Scene* p_scene;
};
}

#endif // SCENETREEWIDGET_H
