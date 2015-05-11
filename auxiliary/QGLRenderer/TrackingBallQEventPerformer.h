#pragma once
#include "QManipulatorEventPerformer.h"

namespace Etoile
{
	class TrackingBallCameraManipulator;
	class TrackingBallQEventPerformer : public QManipulatorEventPerformer
	{
	public:
		TrackingBallQEventPerformer(void);
		~TrackingBallQEventPerformer(void);
		void setManipulator(TrackingBallCameraManipulator* manipulator){p_manipulator = manipulator;}
		TrackingBallCameraManipulator* getManipulator(){return p_manipulator;}
	protected:
		virtual void mousePressEvent(QMouseEvent* const event) override;
		virtual void mouseDoubleClickEvent(QMouseEvent* const event) override;
		virtual void mouseReleaseEvent(QMouseEvent* const event) override;
		virtual void mouseMoveEvent(QMouseEvent* const event) override;
		virtual void wheelEvent(QWheelEvent* const event) override;
		virtual void animate() override;

		TrackingBallCameraManipulator* p_manipulator;
		QPoint m_prevPos, m_pressPos;
		float m_spinsensibility;
	};

}