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
		updateTransform();
	}

	void QTEntityParametersWidget::updateTransform()
	{
		if(NULL == p_entity) return;
		ModelTransform* transform = p_entity->getTransformation();
		Vec3f scale = transform->getScale();
		Vec3f trans = transform->getTranslation();
		Quaternionf q = transform->getRotation();
		Matrix4f mat = transform->getModelMatrix();

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

	}
}