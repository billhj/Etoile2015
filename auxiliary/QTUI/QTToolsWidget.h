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
		QColor getColorButtonCurrentColor();
		QAbstractButton* getCurrentToolsButton();
	public slots:
		void colorButton_onclick();
		void setColorButtonStyleSheet(QString styleSheet);
		void toolsButton_onclick(QAbstractButton* button);
signals:
		void toolsButtonClicked(QAbstractButton* button);
	private:
		Ui::QTToolsWidget ui;
		QColorDialog * p_colordlg;
	};
}

#endif // QTTOOLSWIDGET_H
