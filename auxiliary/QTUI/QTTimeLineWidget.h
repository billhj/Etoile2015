#ifndef QTTIMELINEWIDGET_H
#define QTTIMELINEWIDGET_H

#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QThread>
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
		QTTimeLineWidget* p_parent;
	public:
		QTTimeLineScene(): QGraphicsScene()
		{
			m_range = 200;
			reset();
		}

		void setContainer(QTTimeLineWidget* parent)
		{
			p_parent = parent;
		}

		public slots:
			void reset()
			{
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

				void setRange(int);
				virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent);
				virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
				virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent);
				virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
	};

	class QTTimeLineWidget : public QWidget
	{
		Q_OBJECT
	public:
		QTTimeLineWidget(QWidget *parent = 0);
		~QTTimeLineWidget();
		void init();
		int getActiveFrame();
		int getStartFrame();
		int getEndFrame();
		public slots:
			void setActiveFrame(int frame);
			void setStartFrame(int value);
			void setEndFrame(int value);
signals:
			void activeFrameChanged(int);
	private:
		Ui::QTTimeLineWidget ui;
		QTTimeLineScene m_scene;

	};

}
#endif // QTTIMELINEWIDGET_H
