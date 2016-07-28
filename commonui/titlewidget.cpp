#include "titlewidget.h"

#include "iconhelper.h"

TitleWidget::TitleWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	widget_title = ui.widget_title;
	lab_Ico = ui.lab_Ico;
	lab_Title = ui.lab_Title;

	btnMenu = ui.btnMenu;
	btnMenu_Min = ui.btnMenu_Min;
	btnMenu_Max = ui.btnMenu_Max;
	btnMenu_Close = ui.btnMenu_Close;
}

TitleWidget::~TitleWidget()
{

}