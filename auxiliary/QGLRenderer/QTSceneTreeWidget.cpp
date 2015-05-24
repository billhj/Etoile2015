#include "QTSceneTreeWidget.h"
#include <QStandardItemModel>
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
	QStandardItemModel* standardModel = new QStandardItemModel ;

    QList<QStandardItem *> preparedRow =prepareRow("first", "second", "third");
    QStandardItem *item = standardModel->invisibleRootItem();
    // adding a row to the invisible root item produces a root element
    item->appendRow(preparedRow);

    QList<QStandardItem *> secondRow =prepareRow("111", "222", "333");
    // adding a row to an item starts a subtree
    preparedRow.first()->appendRow(secondRow);

	ui.treeView->setModel(standardModel);
    ui.treeView->expandAll();
}


QTSceneTreeWidget::~QTSceneTreeWidget()
{

}

void QTSceneTreeWidget::updateModel()
{
	if(p_scene == NULL) return;
	for(unsigned int i = 0 ; i < p_scene->getEntities().size(); ++i)
	{
		Entity* entity = p_scene->getEntities()[i];
	}
}

void QTSceneTreeWidget::setScene(Scene* scene)
{
	p_scene = scene;
}
}