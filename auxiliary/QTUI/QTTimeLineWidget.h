#ifndef QTTIMELINEWIDGET_H
#define QTTIMELINEWIDGET_H

#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include "ui_QTTimeLineWidget.h"

namespace Etoile
{
	class QTTimelineItem : public QGraphicsRectItem
	{

		friend class QTTimeLineScene;
		int m_frame;
		QBrush m_brush;
	public:
		QTTimelineItem(qreal x, qreal y, qreal width, qreal height, int frame) : QGraphicsRectItem(x, y, width, height), m_frame(frame)
		{
			init();
		}

		QTTimelineItem(QRectF f, int frame) : QGraphicsRectItem(f), m_frame(frame)
		{
			init();
		}

		void init()
		{
			this->setFlag(QGraphicsItem::ItemIsSelectable);
			this->setZValue(1);
			m_brush.setStyle(Qt::SolidPattern);
			int i = this->rect().x() / this->rect().width();
			m_brush.setColor( i%2 > 0 ? QColor(100,100,100) : QColor(200,200,200));
		}

		void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	};


	class QTTimeLineScene : public QGraphicsScene
	{
		friend class QTTimeLineWidget;

		Q_OBJECT

		int m_range;
		int m_currentFrame;
		std::vector<int> m_selectedFrames;
		//QGraphicsItemGroup * p_group;
		bool m_startSelection;
		QPointF m_start;
		QPointF m_end;
		QTTimeLineWidget* p_parent;
	public:
		QTTimeLineScene(): QGraphicsScene()
		{
			m_range = 200;
			m_currentFrame = 0;
			reset();
		}

		void setContainer(QTTimeLineWidget* parent)
		{
			p_parent = parent;
		}

		public slots:
			void reset()
			{
				m_startSelection = false;
				this->clear();
				for(int i = -100; i < m_range; ++i)
				{
					QTTimelineItem *rect = new QTTimelineItem(QRectF(i * 5, 0, 5, 100), i);
					this->addItem(rect);
					if(i % 20 == 0)
					{
						QGraphicsTextItem* text = this->addText(QString::number(i));
						text->setDefaultTextColor(QColor(200,150,150));
						text->setPos(rect->rect().left() - 3, rect->rect().bottom());
					}
				}
			}

			public slots:
				void setStartEndActive(int start, int end);
				void setRange(int);
				virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);

				virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);

				virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);

				virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
signals:
				void currentFrameChanged(int frameNb);
	};

	class QTTimeLineWidget : public QWidget
	{
		Q_OBJECT
	public:
		QTTimeLineWidget(QWidget *parent = 0);
		~QTTimeLineWidget();
		void init();
		int getSelectedFrame()
		{
			return m_scene.m_currentFrame;
		}
		const std::vector<int>& getSelectedFrames()
		{
			return m_scene.m_selectedFrames;
		}
		
		void setStartValue(int value)
		{
			ui.start->blockSignals(true);
			ui.start->setValue(value);
			ui.start->blockSignals(false);
		}

		void setEndValue(int value)
		{
			ui.end->blockSignals(true);
			ui.end->setValue(value);
			ui.end->blockSignals(false);
		}
public slots:
	void setActiveFrame(int frame);
	void needToSetStartEndActive(int)
	{
		m_scene.setStartEndActive(ui.start->value(), ui.end->value());
	}
	private:
		Ui::QTTimeLineWidget ui;
		QTTimeLineScene m_scene;

	};

}
#endif // QTTIMELINEWIDGET_H
