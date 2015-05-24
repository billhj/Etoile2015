#ifndef QTFILEWIDGET_H
#define QTFILEWIDGET_H

#include <QWidget>
#include "ui_QTFileWidget.h"
#include <QFileSystemModel>
class QTFileWidget : public QWidget
{
	Q_OBJECT

public:
	QTFileWidget(QWidget *parent = 0);
	~QTFileWidget();
	public slots:
		void treeview_onclick(QModelIndex qindex);
		void listview_onclick(QModelIndex qindex);
		void openFile();
		void newFile();
		void removeFile();
private:
	Ui::QTFileWidget ui;
	QFileSystemModel * p_filemodel;
	QFileSystemModel * p_dirmodel;
	QModelIndex m_fileIndex;
	QModelIndex m_dirIndex;
};

#endif // QTFILEWIDGET_H
