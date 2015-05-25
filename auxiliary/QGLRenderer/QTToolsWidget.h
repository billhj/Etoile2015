#ifndef QTTOOLSWIDGET_H
#define QTTOOLSWIDGET_H

#include <QWidget>
#include "ui_QTToolsWidget.h"
#include <QColorDialog>

namespace Etoile
{
	class QTToolsWidget : public QWidget
	{
		Q_OBJECT

	public:
		QTToolsWidget(QWidget *parent = 0);
		~QTToolsWidget();
	public slots:
		void selectColor();
		void setColorButtonStyleSheet(QString styleSheet);
	private:
		Ui::QTToolsWidget ui;
		QColorDialog * p_colordlg;
	};
}

#endif // QTTOOLSWIDGET_H
