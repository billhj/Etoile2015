#include "QTSceneTreeWidget.h"
#include <QFileInfo>
#include <QStringList>
#include <QVariant>
#include "geometry/SceneManager.h"

namespace Etoile
{
	QList<QStandardItem *> prepareRow(const QString &first, const QString &second, const QString &third)
	{
		QList<QStandardItem *> rowItems;
		rowItems << new QStandardItem(first);
		rowItems << new QStandardItem(second);
		rowItems << new QStandardItem(third);
		return rowItems;
	}

	QTSceneTreeWidget::QTSceneTreeWidget(QWidget *parent) 
		: QWidget(parent), p_scene(NULL)
	{
		ui.setupUi(this);
		p_standardModel = new QStandardItemModel;
		QStringList list;
		list.append("Scene");
		p_standardModel->setHorizontalHeaderLabels(list);
		updateModel();
		//QList<QStandardItem *> preparedRow =prepareRow("first", "second", "third");
		//QStandardItem *item = standardModel->invisibleRootItem();
		//// adding a row to the invisible root item produces a root element
		//item->appendRow(preparedRow);

		//QList<QStandardItem *> secondRow =prepareRow("111", "222", "333");
		//// adding a row to an item starts a subtree
		//preparedRow.first()->appendRow(secondRow);

		ui.treeView->setModel(p_standardModel);
		ui.treeView->expandAll();

		setScene(SceneManager::getInstance()->getCurrentScene());
		updateModel();
	}


	QTSceneTreeWidget::~QTSceneTreeWidget()
	{

	}


	void QTSceneTreeWidget::updateModel()
	{
		if(p_scene == NULL) return;
		p_standardModel->clear();
		QStandardItem *item = p_standardModel->invisibleRootItem();
		QStandardItem * sceneRow = new QStandardItem(QString().fromStdString(p_scene->getName()));
		item->appendRow(sceneRow);
		for(unsigned int i = 0 ; i < p_scene->getEntities().size(); ++i)
		{
			Entity* entity = p_scene->getEntities()[i];
			QFileInfo file(QString().fromStdString(entity->getName()));
			QStandardItem * row = new QStandardItem(QString::number(entity->getIndex()).append(" ").append(file.fileName()).append(" ").append(QString().fromStdString(entity->getName())));
			EntityInfo info;
			info.p_entity = entity;
			row->setData(QVariant::fromValue(info));
			sceneRow->appendRow(row);
		}
		ui.treeView->expandAll();
		//ui.treeView->selectionModel()->setCurrentIndex(m_currentModelIndex, QItemSelectionModel::NoUpdate);
		ui.treeView->setExpanded(m_currentModelIndex, true);
		//ui.treeView->expand(m_currentModelIndex);
	}

	void QTSceneTreeWidget::setScene(Scene* scene)
	{
		p_scene = scene;
	}


	void QTSceneTreeWidget::treeview_onclick(QModelIndex index)
	{
		m_currentModelIndex = index;
		QStandardItem *	item = p_standardModel->itemFromIndex(index);
		EntityInfo info = item->data().value<EntityInfo>();
		if(NULL == info.p_entity) return;
		emit entitySelected(info.p_entity->getIndex());
	}
}