#include "TexturePainter.h"
#include <qlabel.h>
#include <QFileDialog>
TexturePainter::TexturePainter(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{

	ui.setupUi(this);
	this->showMaximized();
	p_colordlg = new QColorDialog(this);
	p_colordlg->setWindowTitle(QColorDialog::tr("Select color"));
	ui.colorButton->setAutoFillBackground(true);
	QString styleSheet = QString("background-color: rgb(0, 0, 0);");
	ui.colorButton->setStyleSheet(styleSheet);

	ui.cmdField->appendPlainText(QString("start"));
}

TexturePainter::~TexturePainter()
{

}


void TexturePainter::selectColor()
{
	QPalette pal = ui.colorButton->palette();
	QColor input = pal.color(QPalette::Button);
	p_colordlg->setCurrentColor(input);
	int resultCode = p_colordlg->exec();
	QColor result = input;
	if (resultCode == QDialog::Accepted)
		result = p_colordlg->currentColor();
	QString styleSheet = QString("background-color: rgb(%1, %2, %3);").arg(result.red()).arg(result.green()).arg(result.blue());
	ui.colorButton->setStyleSheet(styleSheet);
}

void TexturePainter::pipetteColorPicked()
{
	QColor result;
	result.setRgbF(ui.centralWidget->m_pixel[0], ui.centralWidget->m_pixel[1], ui.centralWidget->m_pixel[2]);
	QString styleSheet = QString("background-color: rgb(%1, %2, %3);").arg(result.red()).arg(result.green()).arg(result.blue());
	ui.colorButton->setStyleSheet(styleSheet);
}

void TexturePainter::updateOutputError()
{
	QByteArray data;
	ui.cmdField->appendPlainText(QString(data));
}

void TexturePainter::updateOutputText()
{
	QByteArray data;
	ui.cmdField->appendPlainText(QString(data));
}

#include "meshIO/OBJMeshLoader.h"
#include "renderer/OpenGL/ImmediateMeshRenderer.h"
#include "renderer/OpenGL/VBOMeshRenderer.h"
#include "geometry/RenderManager.h"

void TexturePainter::addMesh()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Mesh (*.obj)"));
	OBJMeshLoader loader;

	RenderMesh* mesh = new RenderMesh(name.toStdString());
	loader.loadFromFile(name.toStdString(), mesh);

	VBOMeshRenderer* renderer = new VBOMeshRenderer();
	renderer->setRenderMesh(mesh);

	Entity* entity = new Entity(name.toStdString());
	entity->setComponent(ComponentType::RENDER_COMPONENT, renderer);
	RenderManager::getInstance()->addIntoObjectRendererList(renderer);

}