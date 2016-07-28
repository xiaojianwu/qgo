#include "frmmessagebox.h"
#include "ui_frmmessagebox.h"
#include "iconhelper.h"
#include "myhelper.h"

frmMessageBox::frmMessageBox(QWidget *parent) :
	BaseDialog(parent),
    ui(new Ui::frmMessageBox)
{
	ui->setupUi(this);

    //设置窗体关闭时自动释放内存
    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    //窗体居中显示
    myHelper::FormInCenter(this, parent);
}

frmMessageBox::~frmMessageBox()
{
    delete ui;
}

void frmMessageBox::SetMessage(const QString &msg, int type)
{
	QString title;
    if (type == 0) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/Resources/image/info.png);");
        ui->btnCancel->setVisible(false);
		title = tr("information");
    } else if (type == 1) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/Resources/image/info.png);");
		title = tr("question");
    } else if (type == 2) {
        ui->labIcoMain->setStyleSheet("border-image: url(:/Resources/image/error.png);");
        ui->btnCancel->setVisible(false);
		title = tr("error");
    }

	InitStyle(SYS_BTN_STYLE_CLOSE, ui->widget_title, title, ":/Resources/image/logo.png");

    ui->labInfo->setText(msg);
}

void frmMessageBox::on_btnOk_clicked()
{
    done(1);
    close();
}