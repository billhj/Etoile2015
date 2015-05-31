#include "QTIOWidget.h"

namespace Etoile
{

	void outcallback( const char* ptr, std::streamsize count, void* pTextBox )
	{
		(void) count;
		QPlainTextEdit* p = static_cast<QPlainTextEdit*>( pTextBox );
		//std::string in = std::string(ptr);
		/*if(in.compare("\n")==0){
			std::string m =in;
		}*/
		//std::string out = out + in;
		p->appendPlainText(ptr);
		//p->setPlainText(QString().fromStdString(out));
	}

	QTIOWidget::QTIOWidget(QWidget *parent)
		: QWidget(parent)
	{
		ui.setupUi(this);
		p_redirector = NULL;
		//p_redirector = new StdRedirector<>( std::cout, outcallback, ui.ioTextEdit);
	}

	QTIOWidget::~QTIOWidget()
	{
		if(p_redirector != NULL)
			delete p_redirector;
	}
}
