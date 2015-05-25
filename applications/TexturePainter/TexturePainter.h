#ifndef TEXTUREPAINTER_H
#define TEXTUREPAINTER_H

#include <QtGui/QMainWindow>
#include "ui_TexturePainter.h"
class TexturePainter : public QMainWindow
{
	Q_OBJECT

public:
	TexturePainter(QWidget *parent = 0, Qt::WFlags flags = 0);
	~TexturePainter();
	void load();
	void save();
protected:
    void closeEvent(QCloseEvent *event);
	public slots:
		void pipetteColorPicked();
		void addMesh();
		void updateOutputError();
		void updateOutputText();

private:
	Ui::TexturePainterClass ui;
	QString m_fileDir;
};

#endif // TEXTUREPAINTER_H
