#include "TrackingBallQEventPerformer.h"
#include"QManipulatorEventManager.h"
#include "geometry/TrackingBallCameraManipulator.h"
#include "geometry/Camera.h"

namespace Etoile
{

	TrackingBallQEventPerformer::TrackingBallQEventPerformer(void) : QManipulatorEventPerformer()
	{
		QManipulatorEventManager::getInstance()->addPerformer(this);
		m_spinsensibility = 6;
	}


	TrackingBallQEventPerformer::~TrackingBallQEventPerformer(void)
	{
	}

	void TrackingBallQEventPerformer::mousePressEvent(QMouseEvent* const event)
	{
		if (event->button() == Qt::MouseButton::LeftButton){
			p_manipulator->m_move = false;
			p_manipulator->m_rotate = true;
			p_manipulator->m_zoom = false;
		}
		else if (event->button() == Qt::MouseButton::MiddleButton){
			p_manipulator->m_move = false;
			p_manipulator->m_rotate = false;
			p_manipulator->m_zoom = true;
		}
		else if (event->button() == Qt::MouseButton::RightButton){
			p_manipulator->m_move = true;
			p_manipulator->m_rotate = false;
			p_manipulator->m_zoom = false;
		}
		m_prevPos = m_pressPos = event->pos();
	}

	void TrackingBallQEventPerformer::mouseDoubleClickEvent(QMouseEvent* const event)
	{
	}

	void TrackingBallQEventPerformer::mouseReleaseEvent(QMouseEvent* const event)
	{
		p_manipulator->m_move = false;
		p_manipulator->m_rotate = false;
		p_manipulator->m_zoom = false;
	}

	void TrackingBallQEventPerformer::mouseMoveEvent(QMouseEvent* const event)
	{
		QPoint currentPos = event->pos();
		if(p_manipulator->m_move)
		{
			QPoint delta = m_prevPos - event->pos();
			p_manipulator->moveOnScreen(delta.x(), delta.y());
		}
		else if(p_manipulator->m_rotate)
		{
			p_manipulator->rotateOnScreen(m_prevPos.x(), m_prevPos.y(), currentPos.x(), currentPos.y());
			if( (currentPos - m_prevPos).manhattanLength() > m_spinsensibility )
			{
				p_manipulator->setSpinActive(true);
			}else
			{
				p_manipulator->setSpinActive(false);
			}
		}
		else if(p_manipulator->m_zoom)
		{
			qreal dx = qreal(event->x() - m_prevPos.x()) / p_manipulator->getCamera()->getWidth();
			qreal dy = qreal(event->y() - m_prevPos.y()) / p_manipulator->getCamera()->getHeight();
			qreal value = fabs(dx) > fabs(dy) ? dx : dy;
			p_manipulator->zoom(value);
		}

		m_prevPos = currentPos;
	}

	void TrackingBallQEventPerformer::wheelEvent(QWheelEvent* const event)
	{
		static const qreal WHEEL_SENSITIVITY_COEF = 8E-4;
		float value = event->delta() * p_manipulator->wheelSensitivity() * WHEEL_SENSITIVITY_COEF;
		p_manipulator->zoom(value);
	}

	void TrackingBallQEventPerformer::animate()
	{
		if(p_manipulator != NULL) p_manipulator->spinUpdate();
	}
}