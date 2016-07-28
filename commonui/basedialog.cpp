#include "basedialog.h"

#include <QCoreApplication>
#include <QDesktopWidget>

#include <QMessageBox>
#include "iconhelper.h"

#include <QDebug>

BaseDialog::BaseDialog(QWidget *parent)
	: QDialog(parent)
	, m_titleWidget(0)
{
}

BaseDialog::~BaseDialog()
{

}

QWidget* BaseDialog::getMainWidget()
{
	return NULL;
}

void BaseDialog::setSysBtnSytle(SYS_BTN_STYLE style, bool showMenu)
{
	m_titleWidget->btnMenu_Close->hide();
	m_titleWidget->btnMenu_Max->hide();
	m_titleWidget->btnMenu_Min->hide();

	if (!showMenu)
	{
		m_titleWidget->btnMenu->hide();
	}
	

	switch (style)
	{
	case SYS_BTN_STYLE_NORAMAL:
	{
		m_titleWidget->btnMenu_Close->show();
		m_titleWidget->btnMenu_Max->show();
		m_titleWidget->btnMenu_Min->show();
	}
		break;
	case SYS_BTN_STYLE_MIN:
	{
		m_titleWidget->btnMenu_Close->show();
		m_titleWidget->btnMenu_Min->show();
	}
		break;
	case SYS_BTN_STYLE_CLOSE:
	{
		m_titleWidget->btnMenu_Close->show();
	}
		break;
	case SYS_BTN_STYLE_NONE:
		break;
	case SYS_BTN_STYLE_DEFALUT:
	default:
	{
		m_titleWidget->btnMenu_Close->show();
		m_titleWidget->btnMenu_Max->show();
		m_titleWidget->btnMenu_Min->show();
		m_titleWidget->btnMenu->show();
	}
		break;
	}
}

void BaseDialog::InitStyle(SYS_BTN_STYLE style, 
						   QWidget *titleContainer, 
	                       QString title, 
	                       QString iconPath, 
	                       bool showMenu)
{
	//设置窗体标题栏隐藏
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
	location = this->geometry();
	m_bMax = false;
	mousePressed = false;

	if (titleContainer == NULL)
	{
		return;
	}
	m_titleContainer = titleContainer;
	m_titleWidget = new TitleWidget(m_titleContainer);

	connect(m_titleWidget->btnMenu_Close, SIGNAL(clicked()), this, SLOT(onCloseClicked()));
	connect(m_titleWidget->btnMenu_Max, SIGNAL(clicked()), this, SLOT(onMaxClicked()));
	connect(m_titleWidget->btnMenu_Min, SIGNAL(clicked()), this, SLOT(onMinClicked()));
	connect(m_titleWidget->btnMenu, SIGNAL(clicked()), this, SLOT(onMenuClicked()));

	//安装事件监听器,让标题栏识别鼠标双击
	m_titleWidget->lab_Title->installEventFilter(this);

	IconHelper::Instance()->SetIcon(m_titleWidget->btnMenu_Close, QChar(0xf00d), 10);
	IconHelper::Instance()->SetIcon(m_titleWidget->btnMenu_Max, QChar(0xf096), 10);
	IconHelper::Instance()->SetIcon(m_titleWidget->btnMenu_Min, QChar(0xf068), 10);
	IconHelper::Instance()->SetIcon(m_titleWidget->btnMenu, QChar(0xf0c9), 10);

	if (iconPath.isEmpty())
	{
		IconHelper::Instance()->SetIcon(m_titleWidget->lab_Ico, QChar(0xf015), 12);
	}
	else
	{
		IconHelper::Instance()->SetIcon(m_titleWidget->lab_Ico, iconPath);
	}
	

	setSysBtnSytle(style, showMenu);

	m_titleWidget->lab_Title->setText(title);
	QDialog::setWindowTitle(title);

	m_titleWidget->btnMenu->setShortcut(Qt::CTRL + Qt::Key_Q);
}

bool BaseDialog::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonDblClick) {
		if (!isFullScreen())
		{
			this->onMaxClicked();
			return true;
		}
	}
	return QObject::eventFilter(obj, event);
}

void BaseDialog::mouseMoveEvent(QMouseEvent *e)
{
	if (mousePressed && (e->buttons() && Qt::LeftButton) && !m_bMax) {
		this->move(e->globalPos() - mousePoint);
		e->accept();
	}
}

void BaseDialog::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		mousePressed = true;
		mousePoint = e->globalPos() - this->pos();
		e->accept();
	}
}

void BaseDialog::mouseReleaseEvent(QMouseEvent *)
{
	mousePressed = false;
}

void BaseDialog::resizeEvent(QResizeEvent *e)
{
	e->accept();
	if (m_titleWidget)
	{
		//qDebug() << "m_titleContainer size=" << m_titleContainer->size();
		//m_titleWidget->widget_title->setFixedSize(m_titleContainer->size());
		m_titleWidget->setFixedSize(m_titleContainer->size());
	}
	
}

void BaseDialog::onCloseClicked()
{
	//qApp->exit();
	close();
}

void BaseDialog::onMaxClicked()
{
	showMaximized();
}

void BaseDialog::onMinClicked()
{
	QDialog::showMinimized();
}

void BaseDialog::onMenuClicked()
{
	QMessageBox::information(this, "not defined", "not defined");
}

void BaseDialog::showFullScreen()
{
	m_bMax = true;

	//QDesktopWidget w;
	//QRect r = w.screenGeometry();
	//qDebug() << r.width() << r.height();

	//this->setGeometry(r);
	//show();
	QDialog::showFullScreen();
}

void BaseDialog::showMaximized()
{
	if (m_bMax) {
		this->setGeometry(location);
		IconHelper::Instance()->SetIcon(m_titleWidget->btnMenu_Max, QChar(0xf096), 10);
		m_titleWidget->btnMenu_Max->setToolTip(tr("max"));
	}
	else {
		location = this->geometry();
		this->setGeometry(qApp->desktop()->availableGeometry());
		IconHelper::Instance()->SetIcon(m_titleWidget->btnMenu_Max, QChar(0xf079), 10);
		m_titleWidget->btnMenu_Max->setToolTip(tr("restore"));
	}
	show();
	m_bMax = !m_bMax;
}

