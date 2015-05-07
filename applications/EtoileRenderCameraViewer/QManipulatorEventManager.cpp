#include "QManipulatorEventManager.h"

namespace Etoile
{
	QManipulatorEventManager::QManipulatorEventManager(void)
	{
	}

	void QManipulatorEventManager::addPerformer(QManipulatorEventPerformer* performer)
	{
		for(unsigned int i = 0; i < m_performers.size(); ++i)
		{
			if(performer == m_performers[i])
			{
				return;
			}
		}
		m_performers.push_back(performer);
	}

	void QManipulatorEventManager::removePerformer(QManipulatorEventPerformer* performer)
	{
		for(unsigned int i = 0; i < m_performers.size(); ++i)
		{
			if(performer == m_performers[i])
			{
				m_performers.erase(m_performers.begin() + i);
				return;
			}
		}
	}

	void QManipulatorEventManager::performMousePressEvent(QMouseEvent* const event)
	{
		for(unsigned int i = 0; i < m_performers.size(); ++i)
		{
			if(NULL != m_performers[i] && m_performers[i]->isActive())
			{
				m_performers[i]->mousePressEvent(event);
			}
		}
	}

	void QManipulatorEventManager::performMouseDoubleClickEvent(QMouseEvent* const event)
	{
		for(unsigned int i = 0; i < m_performers.size(); ++i)
		{
			if(NULL != m_performers[i] && m_performers[i]->isActive())
			{
				m_performers[i]->mouseDoubleClickEvent(event);
			}
		}
	}

	void QManipulatorEventManager::performMouseReleaseEvent(QMouseEvent* const event)
	{
		for(unsigned int i = 0; i < m_performers.size(); ++i)
		{
			if(NULL != m_performers[i] && m_performers[i]->isActive())
			{
				m_performers[i]->mouseReleaseEvent(event);
			}
		}
	}

	void QManipulatorEventManager::performMouseMoveEvent(QMouseEvent* const event)
	{
		for(unsigned int i = 0; i < m_performers.size(); ++i)
		{
			if(NULL != m_performers[i] && m_performers[i]->isActive())
			{
				m_performers[i]->mouseMoveEvent(event);
			}
		}
	}

	void QManipulatorEventManager::performWheelEvent(QWheelEvent* const event)
	{
		for(unsigned int i = 0; i < m_performers.size(); ++i)
		{
			if(NULL != m_performers[i] && m_performers[i]->isActive())
			{
				m_performers[i]->wheelEvent(event);
			}
		}
	}

	void QManipulatorEventManager::performAnimate()
	{
		for(unsigned int i = 0; i < m_performers.size(); ++i)
		{
			if(NULL != m_performers[i] && m_performers[i]->isActive())
			{
				m_performers[i]->animate();
			}
		}
	}

}