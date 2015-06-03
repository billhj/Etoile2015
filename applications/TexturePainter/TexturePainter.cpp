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
	connect(ui.fileView, SIGNAL(fileToOpen(QString)), this, SLOT(openFile(QString)));
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

#include "meshIO/GeneralLoader.h"

void TexturePainter::openFile()
{
	QString name = QFileDialog::getOpenFileName(this, tr("Open File"),"",tr("Mesh (*.obj)"));
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

void TexturePainter::openFile(QString file)
{
	GeneralLoader::loadFromFile(file.toStdString());
	Scene* scene = SceneManager::getInstance()->getCurrentScene();
	ui.sceneTreeView->setScene(scene);ui.sceneTreeView->updateModel();
}

void TexturePainter::closeEvent(QCloseEvent *event)
{
	save();
	QMainWindow::closeEvent(event);
}