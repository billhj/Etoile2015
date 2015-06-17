#include "QTTimeLineWidget.h"
#include <QGraphicsRectItem>


namespace Etoile
{
	int startFrame = 0;;
	int endFrame = 100;
	int activeFrame = 0;

	void QTTimelineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget)
	{
		painter->setPen(this->pen());
		if(activeFrame == m_frame)
		{
			m_brush.setColor(Qt::red);
		}
		else if(startFrame <= m_frame && m_frame <= endFrame)
		{
			m_brush.setColor(Qt::cyan);
			painter->drawText(this->rect().x(), this->rect().y(), 100, 80, Qt::AlignHCenter, QString::number(m_frame));
		}else
		{
			int i = this->rect().x() / this->rect().width();
			m_brush.setColor( i%2 == 0 ? QColor(100,100,100) : QColor(200,200,200));
		}

		painter->setBrush(m_brush);
		painter->drawRect(this->rect());
	}


	QTTimeLineWidget::QTTimeLineWidget(QWidget *parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
		ui.graphicsView->setScene(&m_scene);
		m_scene.setContainer(this);
		//ui.graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
		//ui.graphicsView->setRenderHint(QPainter::Antialiasing, true);
		init();
		p_timer = new QTimer(this);
		connect(p_timer, SIGNAL(timeout()), this, SLOT(updateActiveFrame()));
		
	}

	void QTTimeLineWidget::updateActiveFrame()
	{
		activeFrame = ((double)activeFrame) + ui.speed->value();
		if(activeFrame > endFrame)
		{
			activeFrame = startFrame;
		}
		setActiveFrame(activeFrame);
	}

	void QTTimeLineWidget::startTimer()
	{
		p_timer->start(20);
	}

	void QTTimeLineWidget::stopTimer()
	{
		p_timer->stop();
	}

	void QTTimeLineWidget::init()
	{
		connect(this, SIGNAL(activeFrameChanged(int)), this->ui.current, SLOT(setValue(int)));
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
		if(mouseEvent->buttons() == Qt::LeftButton)
		{
			foreach(QGraphicsItem* item , this->items())
			{
				if(item->boundingRect().contains(mouseEvent->scenePos()))
				{
					p_parent->setActiveFrame(((QTTimelineItem*)item)->m_frame);
					break;
				}
			}
			
		}
	}

	void QTTimeLineScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
	{	
		foreach(QGraphicsItem* item , this->items())
		{
			if(NULL!=dynamic_cast<QTTimelineItem*>(item))
			{
				if(item->boundingRect().contains(mouseEvent->scenePos()))
				{
					p_parent->setActiveFrame(((QTTimelineItem*)item)->m_frame);
					break;
				}
			}
		}

		//p_parent->setActiveFrame(activeFrame);
		//QGraphicsScene::mousePressEvent(mouseEvent);
	}

	void QTTimeLineScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
	{
		//	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	}

	void QTTimeLineScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent)
	{
		/*foreach(QGraphicsItem* item , this->items())
		{
		item->setSelected(false);
		}*/
		QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
	}


	void QTTimeLineWidget::setActiveFrame(int frame)
	{
		activeFrame = frame;
		if(ui.current->value() != frame)
		{
			ui.current->blockSignals(true);
			ui.current->setValue(frame);
			ui.current->blockSignals(false);
		}
		if(activeFrame > m_scene.m_range - 50)
		{
			m_scene.setRange(m_scene.m_range + 100);
		}
		
		m_scene.update();
		this->update();
		emit activeFrameChanged(activeFrame);
	}

	int QTTimeLineWidget::getActiveFrame()
	{
		return activeFrame;
	}

	int QTTimeLineWidget::getStartFrame()
	{
		return startFrame;
	}

	int QTTimeLineWidget::getEndFrame()
	{
		return endFrame;
	}

	void QTTimeLineWidget::setStartFrame(int value)
	{
		startFrame = value;
		if(ui.start->value() != value)
		{
			ui.start->blockSignals(true);
			ui.start->setValue(value);
			ui.start->blockSignals(false);
		}
		if(startFrame > m_scene.m_range - 50)
		{
			m_scene.setRange(m_scene.m_range + 100);
		}
		this->update();
	}

	void QTTimeLineWidget::setEndFrame(int value)
	{
		endFrame = value;
		if(ui.end->value() != value)
		{
			ui.end->blockSignals(true);
			ui.end->setValue(value);
			ui.end->blockSignals(false);
		}
		if(endFrame > m_scene.m_range - 50)
		{
			m_scene.setRange(m_scene.m_range + 100);
		}
		this->update();
	}
}