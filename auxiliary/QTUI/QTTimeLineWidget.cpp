#include "QTTimeLineWidget.h"
#include <QGraphicsRectItem>

namespace Etoile
{
	QTTimeLineWidget::QTTimeLineWidget(QWidget *parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
		ui.graphicsView->setScene(&m_scene);
		//ui.graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
		//ui.graphicsView->setRenderHint(QPainter::Antialiasing, true);
		init();
	}

	void QTTimeLineWidget::init()
	{
		connect(&m_scene, SIGNAL(currentFrameChanged(int)), this->ui.current, SLOT(setValue(int)));
	}

	QTTimeLineWidget::~QTTimeLineWidget()
	{

	}

	void QTTimeLineScene::setRange(int value)
	{
		m_range = value;
		reset();
	}

	void QTTimeLineScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
	{
		if(m_startSelection)
		{
			m_end = mouseEvent->scenePos();
			foreach(QGraphicsItem* item , this->items())
			{
				if(item->boundingRect().intersects(QRectF(m_start, m_end)))
				{
					item->setSelected(true);
				}else
				{
					item->setSelected(false);
				}
			}
		}
	}

	void QTTimeLineScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
	{	

		foreach(QGraphicsItem* item , this->items())
		{
			item->setSelected(false);
		}
		m_startSelection = true;
		m_start = mouseEvent->scenePos();
		if(m_startSelection)
		{
			m_end = mouseEvent->scenePos();
			foreach(QGraphicsItem* item , this->items())
			{
				if(item->boundingRect().contains(m_start))
				{
					item->setSelected(true);
					m_currentFrame = ((QTTimelineItem*)item)->m_frame;

				}else
				{
					item->setSelected(false);
				}
			}
		}
		emit currentFrameChanged(m_currentFrame);
		if(m_currentFrame > m_range - 100)
		{
			setRange(m_range + 100);
		}
		//QGraphicsScene::mousePressEvent(mouseEvent);
	}

	void QTTimeLineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
	{
		m_startSelection = false;
		//	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	}

	void QTTimeLineScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
	{
		/*foreach(QGraphicsItem* item , this->items())
		{
		item->setSelected(false);
		}*/
		//QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
	}


}