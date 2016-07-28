#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDialog>

#include "QMouseEvent"

#include "titlewidget.h"

enum SYS_BTN_STYLE
{
	SYS_BTN_STYLE_DEFALUT = 0, // �˵�,��С������󻯣��ر�
	SYS_BTN_STYLE_NORAMAL, // ��С������󻯣��ر�
	SYS_BTN_STYLE_MIN, // ��С�����ر�
	SYS_BTN_STYLE_CLOSE, // �ر�
	SYS_BTN_STYLE_NONE // ʲô��û
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
