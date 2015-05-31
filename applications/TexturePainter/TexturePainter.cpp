#include "TexturePainter.h"
#include <qlabel.h>
#include <QFileDialog>
#include <QSettings>
#include "renderer/OpenGL/GLTextureCreator.h"
#include "image/GeneralImageLoader.h"

TexturePainter::TexturePainter(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	load();
	this->showMaximized();
	ImageManager::getInstance()->setImageLoader(new GeneralImageLoader());
	TextureManager::getInstance()->setTextureCreator(new GLTextureCreator());
}

TexturePainter::~TexturePainter()
{

}


void TexturePainter::load()
{
	m_fileDir = QDir::currentPath();
	QSettings settings("TexturePainter.ini", QSettings::IniFormat);
	m_fileDir = settings.value("filepath").toString();	
	restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());

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
	settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
	settings.sync();

	
  
}

void TexturePainter::pipetteColorPicked()
{
	QColor result;
	result.setRgbF(ui.centralWidget->m_pixel[0], ui.centralWidget->m_pixel[1], ui.centralWidget->m_pixel[2]);
	QString styleSheet = QString("background-color: rgb(%1, %2, %3);").arg(result.red()).arg(result.green()).arg(result.blue());
	ui.toolsWidget->setColorButtonStyleSheet(styleSheet);
}

#include "meshIO/OBJMeshLoader.h"
#include "renderer/OpenGL/ImmediateMeshRenderer.h"
#include "renderer/OpenGL/VBOMeshRenderer.h"
#include "geometry/RenderManager.h"
#include "geometry/SceneManager.h"
//#include "QTUI/QTTextureLoader.h"


void TexturePainter::addMesh()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Mesh (*.obj)"));
	//QFile file(name);
	if(name.isEmpty()) return;
	OBJMeshLoader loader;
	/*QTTextureLoader qtextureLoader;
	loader.setTextureLoader(&qtextureLoader);*/

	RenderMesh* mesh = new RenderMesh(name.toStdString());
	loader.loadFromFile(name.toStdString(), mesh);

	VBOMeshRenderer* renderer = new VBOMeshRenderer();
	renderer->setRenderMesh(mesh);
	Scene* scene = SceneManager::getInstance()->getCurrentScene();
	Entity* entity = new Entity(name.toStdString(), scene);
	entity->setComponent(ComponentType::RENDER_COMPONENT, renderer);
	RenderManager::getInstance()->addIntoObjectRendererList(renderer);
	ui.sceneTreeView->setScene(scene);ui.sceneTreeView->updateModel();
}

void TexturePainter::closeEvent(QCloseEvent *event)
{
	save();
	QMainWindow::closeEvent(event);
}