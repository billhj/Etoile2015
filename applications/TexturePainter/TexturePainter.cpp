#include "TexturePainter.h"
#include <qlabel.h>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QSettings>

TexturePainter::TexturePainter(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	load();
	this->showMaximized();
	p_colordlg = new QColorDialog(this);
	p_colordlg->setWindowTitle(QColorDialog::tr("Select color"));
	ui.colorButton->setAutoFillBackground(true);
	QString styleSheet = QString("background-color: rgb(0, 0, 0);");
	ui.colorButton->setStyleSheet(styleSheet);

	QFileSystemModel *model = new QFileSystemModel;
    model->setRootPath(m_fileDir);
	ui.fileView->setModel(model);
	ui.cmdField->appendPlainText(QString("start"));
}

TexturePainter::~TexturePainter()
{

}


void TexturePainter::load()
{
	m_fileDir = QDir::currentPath();
	QSettings settings("TexturePainter.ini", QSettings::IniFormat);
	m_fileDir = settings.value("filepath").toString();	


	QList<QAction*> windowactions = ui.menuWindows->actions();
	foreach(QAction* action , windowactions)
	{
		action->setChecked(!settings.value(action->objectName()).toBool());
		action->trigger();
	}
}

void TexturePainter::save()
{
	QSettings settings("TexturePainter.ini", QSettings::IniFormat);
    settings.setValue("filepath", m_fileDir);
	QList<QAction*> windowactions = ui.menuWindows->actions();
	foreach(QAction* action , windowactions)
	{
		settings.setValue(action->objectName(), action->isChecked());
	}
	settings.sync();
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
#include "QGLRenderer/QTTextureLoader.h"


void TexturePainter::addMesh()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Mesh (*.obj)"));
	OBJMeshLoader loader;
	QTTextureLoader qtextureLoader;
	loader.setTextureLoader(&qtextureLoader);

	RenderMesh* mesh = new RenderMesh(name.toStdString());
	loader.loadFromFile(name.toStdString(), mesh);

	VBOMeshRenderer* renderer = new VBOMeshRenderer();
	renderer->setRenderMesh(mesh);

	Entity* entity = new Entity(name.toStdString());
	entity->setComponent(ComponentType::RENDER_COMPONENT, renderer);
	RenderManager::getInstance()->addIntoObjectRendererList(renderer);

}

void TexturePainter::closeEvent(QCloseEvent *event)
{
	save();
}