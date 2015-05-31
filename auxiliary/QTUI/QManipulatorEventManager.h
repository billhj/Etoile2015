#pragma once

#include <vector>
#include "QManipulatorEventPerformer.h"

namespace Etoile
{
class QManipulatorEventManager
{
protected:
	std::vector<QManipulatorEventPerformer*> m_performers;
	QManipulatorEventManager(void);
public:
	static QManipulatorEventManager* getInstance()
	{
		static QManipulatorEventManager manager;
		return &manager;
	}
	void addPerformer(QManipulatorEventPerformer*);
	void removePerformer(QManipulatorEventPerformer*);
	void performMousePressEvent(QMouseEvent* const event);
	void performMouseDoubleClickEvent(QMouseEvent* const event);
	void performMouseReleaseEvent(QMouseEvent* const event);
	void performMouseMoveEvent(QMouseEvent* const event);
	void performWheelEvent(QWheelEvent* const event);
	void performAnimate();
};
}
