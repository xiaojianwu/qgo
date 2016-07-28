#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDialog>

#include "QMouseEvent"

#include "titlewidget.h"

enum SYS_BTN_STYLE
{
	SYS_BTN_STYLE_DEFALUT = 0, // 菜单,最小化，最大化，关闭
	SYS_BTN_STYLE_NORAMAL, // 最小化，最大化，关闭
	SYS_BTN_STYLE_MIN, // 最小化，关闭
	SYS_BTN_STYLE_CLOSE, // 关闭
	SYS_BTN_STYLE_NONE // 什么都没
};
class BaseDialog : public QDialog
{
	Q_OBJECT

public:
	BaseDialog(QWidget *parent = 0);
	~BaseDialog();

public:
	QWidget* getMainWidget();
	void InitStyle(SYS_BTN_STYLE style = SYS_BTN_STYLE_NORAMAL, 
				   QWidget *titleContainer = NULL, 
				   QString title = "", 
				   QString iconPath = "",
			       bool showMenu = false);

	void showFullScreen();
	void showMaximized();

protected:
	bool eventFilter(QObject *obj, QEvent *event);
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *);

	void resizeEvent(QResizeEvent *e);
	

protected slots:
	void onCloseClicked();

	void onMaxClicked();

	void onMinClicked();

	void onMenuClicked();
private:
	
	void setSysBtnSytle(SYS_BTN_STYLE style, bool showMenu);

private:
	QPoint mousePoint;
	bool mousePressed;
	bool m_bMax;
	QRect location;
	TitleWidget *m_titleWidget;
	QWidget *m_titleContainer;
};

#endif // BASEDIALOG_H
