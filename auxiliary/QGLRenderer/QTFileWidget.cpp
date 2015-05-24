#include "QTFileWidget.h"

QTFileWidget::QTFileWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QString path = QDir::currentPath();
	
	p_dirmodel = new QFileSystemModel(this);
	p_dirmodel->setRootPath(path);
	p_dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
	ui.treeView->setModel(p_dirmodel);

	p_filemodel = new QFileSystemModel(this);
	p_filemodel->setRootPath(path);
	p_filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
	ui.listView->setModel(p_filemodel);
}

QTFileWidget::~QTFileWidget()
{

}


void QTFileWidget::treeview_onclick(QModelIndex qindex)
{
	QString path = p_dirmodel->fileInfo(qindex).absoluteFilePath();
	ui.listView->setRootIndex(p_filemodel->setRootPath(path));
}