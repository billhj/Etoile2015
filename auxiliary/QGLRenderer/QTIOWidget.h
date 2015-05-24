#ifndef QTIOWIDGET_H
#define QTIOWIDGET_H

#include <QWidget>
#include "ui_QTIOWidget.h"
#include "util/StdRedirector.h"

namespace Etoile
{
	
class QTIOWidget : public QWidget
{
	Q_OBJECT

public:
	QTIOWidget(QWidget *parent = 0);
	~QTIOWidget();

private:
	Ui::QTIOWidget ui;
	StdRedirector<> * p_redirector;
};
}

#endif // QTIOWIDGET_H
