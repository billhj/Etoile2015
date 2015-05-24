#include "QTSceneTreeWidget.h"
#include <QFileInfo>
#include <QStringList>
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
	list.append("name");
	list.append("file");
	list.append("id");
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
}


QTSceneTreeWidget::~QTSceneTreeWidget()
{

}


void QTSceneTreeWidget::updateModel()
{
	if(p_scene == NULL) return;
	p_standardModel->clear();
	QStandardItem *item = p_standardModel->invisibleRootItem();
	QList<QStandardItem *> sceneRow = prepareRow("Scene", QString().fromStdString(p_scene->getName()), "");
	item->appendRow(sceneRow);
	for(unsigned int i = 0 ; i < p_scene->getEntities().size(); ++i)
	{
		Entity* entity = p_scene->getEntities()[i];
		QFileInfo file(QString().fromStdString(entity->getName()));
		QList<QStandardItem *> secondRow = prepareRow(file.fileName(), QString().fromStdString(entity->getName()), QString::number(entity->getIndex()));
		sceneRow.first()->appendRow(secondRow);
	}
}

void QTSceneTreeWidget::setScene(Scene* scene)
{
	p_scene = scene;
}
}