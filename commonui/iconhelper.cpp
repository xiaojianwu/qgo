#include "iconhelper.h"

IconHelper* IconHelper::_instance = 0;
IconHelper::IconHelper(QObject*):
    QObject(qApp)
{
    int fontId = QFontDatabase::addApplicationFont(":/Resources/image/fontawesome-webfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    iconFont = QFont(fontName);
}

void IconHelper::SetIcon(QLabel* lab, QChar c, int size)
{
    iconFont.setPointSize(size);
    lab->setFont(iconFont);
    lab->setText(c);
}

void IconHelper::SetIcon(QPushButton* btn, QChar c, int size)
{
    iconFont.setPointSize(size);
    btn->setFont(iconFont);
    btn->setText(c);
}

void IconHelper::SetIcon(QLabel* lab, QString path)
{
	lab->setPixmap(QPixmap(path));
}

void IconHelper::SetIcon(QPushButton* btn, QString path)
{
	btn->setWindowIcon(QIcon(path));
}
