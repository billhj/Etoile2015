#pragma once

#include <qevent.h>

namespace Etoile
{
	class QManipulatorEventPerformer
	{
		friend class QManipulatorEventManager;
	public:
		QManipulatorEventPerformer(){m_isActive = false;}
		bool isActive()
		{
			return m_isActive;
		}

		void setActive(bool active)
		{
			m_isActive = active;
		}

	protected:
		virtual void mousePressEvent(QMouseEvent* const event) = 0;
		virtual void mouseDoubleClickEvent(QMouseEvent* const event) = 0;
		virtual void mouseReleaseEvent(QMouseEvent* const event)= 0;
		virtual void mouseMoveEvent(QMouseEvent* const event) = 0;
		virtual void wheelEvent(QWheelEvent* const event)= 0;
		virtual void animate() = 0;

		bool m_isActive;
	};
}
