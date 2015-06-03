#include "QTFileWidget.h"
namespace Etoile
{

	QTFileWidget::QTFileWidget(QWidget *parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
		QString path = QDir::currentPath();

		p_filemodel = new QFileSystemModel(this);
		p_filemodel->setRootPath(path);
		p_filemodel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
		ui.listView->setModel(p_filemodel);
		//ui.listView->setRootIndex(p_filemodel->index(path));

		p_dirmodel = new QFileSystemModel(this);
		p_dirmodel->setRootPath(path);
		p_dirmodel->setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
		ui.treeView->setModel(p_dirmodel);
		ui.treeView->expand(p_dirmodel->index(path));
		//ui.treeView->setRootIndex(p_dirmodel->index(path));
		
	}

	QTFileWidget::~QTFileWidget()
	{

	}


	void QTFileWidget::treeview_onclick(QModelIndex qindex)
	{
		m_dirIndex = qindex;
		QString path = p_dirmodel->fileInfo(qindex).absoluteFilePath();
		ui.listView->setRootIndex(p_filemodel->setRootPath(path));
	}

	void QTFileWidget::listview_onclick(QModelIndex qindex)
	{
		m_fileIndex = qindex;
	}

#include <QDebug>

	void QTFileWidget::openFile()
	{
		QString filepath = p_filemodel->filePath(m_fileIndex);
		emit fileToOpen(filepath);
		//qDebug()<<filepath;
	}

	void QTFileWidget::newFile()
	{
		QDir filedir = p_dirmodel->fileInfo(m_dirIndex).absoluteFilePath();
		//qDebug()<<filedir.absolutePath();
	}

	void QTFileWidget::removeFile()
	{
		QString filepath = p_filemodel->filePath(m_fileIndex);
		//qDebug()<<filepath;
	}
}