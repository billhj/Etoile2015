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
	connect(ui.fileView, SIGNAL(fileToOpen(QString)), this, SLOT(openFile(QString)));
	connect(ui.octreeSkeletonView, SIGNAL(filesOpened()), this, SLOT(sceneTreeUpdate()));
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

void SkeletonViewer::openFile()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("File (*.obj; *.sk; *.bvh)"));
	//QFile file(name);
	if(name.isEmpty()) return;
	openFile(name);
	
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
}

void SkeletonViewer::openFile(QString file)
{
	GeneralLoader::loadFromFile(file.toStdString());
	Scene* scene = SceneManager::getInstance()->getCurrentScene();
	ui.sceneTreeView->setScene(scene);ui.sceneTreeView->updateModel();
}
void SkeletonViewer::sceneTreeUpdate()
{
	ui.sceneTreeView->updateModel();
}
void SkeletonViewer::closeEvent(QCloseEvent *event)
{
	save();
	QMainWindow::closeEvent(event);
}

void SkeletonViewer::changeFrameNb(int v)
{
	ui.centralWidget->m_frameNb = v;
}