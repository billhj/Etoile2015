#ifndef QTENTITYPARAMETERSWIDGET_H
#define QTENTITYPARAMETERSWIDGET_H

#include <QWidget>
#include "ui_QTEntityParametersWidget.h"
#include "geometry/Entity.h"

namespace Etoile
{
	class QTEntityParametersWidget : public QWidget
	{
		Q_OBJECT

	public:
		QTEntityParametersWidget(QWidget *parent = 0);
		~QTEntityParametersWidget();
		void setEntity(Entity*);
		public slots:
			void setEntity(int idx);
			/**
			* use current Entity to update widget spinboxes
			*/
			void updateTransformView();
			/**
			*  spinbox to transform value in Entity
			*/
			void scaleSpinBoxModified(double);
			void translationSpinBoxModified(double);
			void quaternionSpinBoxModified(double);
	private:
		Ui::QTEntityParametersWidget ui;
		Entity* p_entity;
	};
}

#endif // QTENTITYPARAMETERSWIDGET_H
