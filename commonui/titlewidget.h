#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include "ui_titlewidget.h"

class TitleWidget : public QWidget
{
	Q_OBJECT

public:
	TitleWidget(QWidget *parent = 0);
	~TitleWidget();


	QWidget *widget_title;

	QLabel *lab_Ico;
	QLabel *lab_Title;

	QPushButton *btnMenu;
	QPushButton *btnMenu_Min;
	QPushButton *btnMenu_Max;
	QPushButton *btnMenu_Close;

private:
	Ui::TitleWidget ui;

};

#endif // TITLEWIDGET_H
