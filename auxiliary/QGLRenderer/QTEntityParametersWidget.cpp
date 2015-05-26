#include "QTEntityParametersWidget.h"
#include "geometry/ModelTransform.h"
#include "geometry/SceneManager.h"

namespace Etoile
{
	QTEntityParametersWidget::QTEntityParametersWidget(QWidget *parent)
		: QWidget(parent), p_entity(NULL)
	{
		ui.setupUi(this);
	}

	QTEntityParametersWidget::~QTEntityParametersWidget()
	{

	}

	void QTEntityParametersWidget::setEntity(Entity* entity)
	{
		p_entity = entity;
	}

	void QTEntityParametersWidget::setEntity(int idx)
	{
		Entity* entity = SceneManager::getInstance()->getCurrentScene()->getEntity(idx);
		setEntity(entity);
		ui.titleEdit->setText(QString::fromStdString(entity->getName()));
		updateTransformView();
	}

	void QTEntityParametersWidget::updateTransformView()
	{
		if(NULL == p_entity) return;
		ModelTransform* transform = p_entity->getTransformation();
		Vec3f scale = transform->getScale();
		Vec3f trans = transform->getTranslation();
		Quaternionf q = transform->getRotation();
		Matrix4f mat = transform->getModelMatrix();

		ui.s0->blockSignals(true);
		ui.s1->blockSignals(true);
		ui.s2->blockSignals(true);
		ui.t0->blockSignals(true);
		ui.t1->blockSignals(true);
		ui.t2->blockSignals(true);
		ui.qx->blockSignals(true);
		ui.qy->blockSignals(true);
		ui.qz->blockSignals(true);
		ui.qw->blockSignals(true);

		ui.s0->setValue(scale.x());
		ui.s1->setValue(scale.y());
		ui.s2->setValue(scale.z());

		ui.t0->setValue(trans.x());
		ui.t1->setValue(trans.y());
		ui.t2->setValue(trans.z());

		ui.qx->setValue(q.x());
		ui.qy->setValue(q.y());
		ui.qz->setValue(q.z());
		ui.qw->setValue(q.w());

		ui.m0->setValue(mat[0][0]);
		ui.m1->setValue(mat[0][1]);
		ui.m2->setValue(mat[0][2]);
		ui.m3->setValue(mat[0][3]);

		ui.m4->setValue(mat[1][0]);
		ui.m5->setValue(mat[1][1]);
		ui.m6->setValue(mat[1][2]);
		ui.m7->setValue(mat[1][3]);

		ui.m8->setValue(mat[2][0]);
		ui.m9->setValue(mat[2][1]);
		ui.m10->setValue(mat[2][2]);
		ui.m11->setValue(mat[2][3]);

		ui.m12->setValue(mat[3][0]);
		ui.m13->setValue(mat[3][1]);
		ui.m14->setValue(mat[3][2]);
		ui.m15->setValue(mat[3][3]);

		ui.s0->blockSignals(false);
		ui.s1->blockSignals(false);
		ui.s2->blockSignals(false);
		ui.t0->blockSignals(false);
		ui.t1->blockSignals(false);
		ui.t2->blockSignals(false);
		ui.qx->blockSignals(false);
		ui.qy->blockSignals(false);
		ui.qz->blockSignals(false);
		ui.qw->blockSignals(false);

	}

	void QTEntityParametersWidget::scaleSpinBoxModified(double)
	{
		if(NULL == p_entity) return;
		ModelTransform* transform = p_entity->getTransformation();
		transform->setScale(Vec3f(ui.s0->value(), ui.s1->value(), ui.s2->value()));
		transform->updateTransform();
		updateTransformView();
	}

	void QTEntityParametersWidget::translationSpinBoxModified(double)
	{
		if(NULL == p_entity) return;
		ModelTransform* transform = p_entity->getTransformation();
		transform->setTranslation(Vec3f(ui.t0->value(), ui.t1->value(), ui.t2->value()));
		transform->updateTransform();
		updateTransformView();
	}

	void QTEntityParametersWidget::quaternionSpinBoxModified(double)
	{
		if(NULL == p_entity) return;
		ModelTransform* transform = p_entity->getTransformation();
		transform->setRotation(Quaternionf(ui.qx->value(), ui.qy->value(), ui.qz->value(), ui.qw->value()));
		transform->updateTransform();
		updateTransformView();
	}
}