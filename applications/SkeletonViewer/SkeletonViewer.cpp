#include "SkeletonViewer.h"
#include <qlabel.h>
#include <QFileDialog>
#include <QSettings>
#include "renderer/OpenGL/GLTextureCreator.h"
#include "image/GeneralImageLoader.h"

SkeletonViewer::SkeletonViewer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	load();
	this->showMaximized();
	ImageManager::getInstance()->setImageLoader(new GeneralImageLoader());
	TextureManager::getInstance()->setTextureCreator(new GLTextureCreator());
}

SkeletonViewer::~SkeletonViewer()
{

}


void SkeletonViewer::load()
{
	m_fileDir = QDir::currentPath();
	QSettings settings("SkeletonViewer.ini", QSettings::IniFormat);
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

void SkeletonViewer::save()
{
	QSettings settings("SkeletonViewer.ini", QSettings::IniFormat);
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

void SkeletonViewer::pipetteColorPicked()
{
	QColor result;
	result.setRgbF(ui.centralWidget->m_pixel[0], ui.centralWidget->m_pixel[1], ui.centralWidget->m_pixel[2]);
	QString styleSheet = QString("background-color: rgb(%1, %2, %3);").arg(result.red()).arg(result.green()).arg(result.blue());
	ui.toolsWidget->setColorButtonStyleSheet(styleSheet);
}

#include "meshIO/GeneralLoader.h"

void SkeletonViewer::addMesh()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Mesh (*.obj)"));
	//QFile file(name);
	if(name.isEmpty()) return;

	Scene* scene = SceneManager::getInstance()->getCurrentScene();
	/*{
	OBJSimpleMeshLoader loadersimple;
	SimpleMesh* simplemesh = loadersimple.loadFromFile(name.toStdString());
	Entity* entity = new Entity(name.toStdString()+"simple", scene);
	entity->setComponent(ComponentType::MESH_COMPONENT, simplemesh);
	GLSimpleMeshRenderer* glrender = new GLSimpleMeshRenderer();
	glrender->setSimpleMesh(simplemesh);
	entity->setComponent(ComponentType::RENDER_COMPONENT, glrender);
	RenderManager::getInstance()->addIntoObjectRendererList(glrender);
	}*/

	GeneralLoader::loadFromFile(name.toStdString());

	ui.sceneTreeView->setScene(scene);ui.sceneTreeView->updateModel();
}

void SkeletonViewer::closeEvent(QCloseEvent *event)
{
	save();
	QMainWindow::closeEvent(event);
}