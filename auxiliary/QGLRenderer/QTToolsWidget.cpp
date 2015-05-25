#include "QTToolsWidget.h"

namespace Etoile
{
	QTToolsWidget::QTToolsWidget(QWidget *parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
		p_colordlg = new QColorDialog(this);
		p_colordlg->setWindowTitle(QColorDialog::tr("Select color"));
		ui.colorButton->setAutoFillBackground(true);
		QString styleSheet = QString("background-color: rgb(0, 0, 0);");
		ui.colorButton->setStyleSheet(styleSheet);
	}

	QTToolsWidget::~QTToolsWidget()
	{

	}

	QColor QTToolsWidget::getColorButtonCurrentColor()
	{
		QPalette pal = ui.colorButton->palette();
		return pal.color(QPalette::Button);
	}

	QAbstractButton* QTToolsWidget::getCurrentToolsButton()
	{
		return ui.buttonGroup->checkedButton();
	}

	void QTToolsWidget::colorButton_onclick()
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

	void QTToolsWidget::setColorButtonStyleSheet(QString styleSheet)
	{
		ui.colorButton->setStyleSheet(styleSheet);
	}

	void QTToolsWidget::toolsButton_onclick(QAbstractButton* button)
	{
		emit toolsButtonClicked(button);
	}
}