#ifndef SCENETREEWIDGET_H
#define SCENETREEWIDGET_H

#include "ui_QTSceneTreeWidget.h"
#include "geometry/Scene.h"
#include <QStandardItemModel>

namespace Etoile
{
	class QTSceneTreeWidget : public QWidget
	{
		Q_OBJECT

	public:
		QTSceneTreeWidget(QWidget *parent = 0);
		~QTSceneTreeWidget();
		void setScene(Scene* scene);
		public slots:
			void updateModel();
			void treeview_onclick(QModelIndex);
signals:
			void entitySelected(int idx);
	private:
		Ui::QTSceneTreeWidget ui;
		QStandardItemModel* p_standardModel;
		Scene* p_scene;
		QModelIndex m_currentModelIndex;
	};

	struct EntityInfo
	{
		Entity* p_entity;
	};

	Q_DECLARE_METATYPE(EntityInfo)
}

#endif // SCENETREEWIDGET_H
