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
private:
	Ui::TexturePainterClass ui;
};

#endif // TEXTUREPAINTER_H
