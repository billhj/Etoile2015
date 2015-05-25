#ifndef QTENTITYPARAMETERSWIDGET_H
#define QTENTITYPARAMETERSWIDGET_H

#include <QWidget>
#include "ui_QTEntityParametersWidget.h"

class QTEntityParametersWidget : public QWidget
{
	Q_OBJECT

public:
	QTEntityParametersWidget(QWidget *parent = 0);
	~QTEntityParametersWidget();

private:
	Ui::QTEntityParametersWidget ui;
};

#endif // QTENTITYPARAMETERSWIDGET_H
