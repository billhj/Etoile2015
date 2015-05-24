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
private:
	Ui::QTFileWidget ui;
	QFileSystemModel * p_filemodel;
	QFileSystemModel * p_dirmodel;
};

#endif // QTFILEWIDGET_H
