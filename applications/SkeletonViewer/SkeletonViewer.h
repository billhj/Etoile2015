#ifndef SkeletonViewer_H
#define SkeletonViewer_H

#include <QtGui/QMainWindow>
#include "ui_SkeletonViewer.h"
class SkeletonViewer : public QMainWindow
{
	Q_OBJECT

public:
	SkeletonViewer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~SkeletonViewer();
	void load();
	void save();
protected:
    void closeEvent(QCloseEvent *event);
	public slots:
		void pipetteColorPicked();
		void addMesh();
private:
	Ui::SkeletonViewerClass ui;
	QString m_fileDir;
};

#endif // SkeletonViewer_H
