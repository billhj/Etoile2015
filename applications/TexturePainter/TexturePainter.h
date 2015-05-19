#ifndef TEXTUREPAINTER_H
#define TEXTUREPAINTER_H

#include <QtGui/QMainWindow>
#include "ui_TexturePainter.h"
#include <QColorDialog>

class TexturePainter : public QMainWindow
{
	Q_OBJECT

public:
	TexturePainter(QWidget *parent = 0, Qt::WFlags flags = 0);
	~TexturePainter();
	public slots:
		void selectColor();
		void pipetteColorPicked();
		void addMesh();
		void updateOutputError();
		void updateOutputText();
private:
	Ui::TexturePainterClass ui;
	QColorDialog * p_colordlg;
};

#endif // TEXTUREPAINTER_H
