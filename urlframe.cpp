#include "urlframe.h"
#include "ui_urlframe.h"

URLFrame::URLFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::URLFrame)
{
    ui->setupUi(this);
    ui->status->clear();
}

URLFrame::~URLFrame()
{
    delete ui;
}

QUrl URLFrame::GetUrl()
{
    QUrl url = ui->url_lineedit->text();
    return url;
}

void URLFrame::SetUrl(QUrl url)
{
    ui->url_lineedit->setText(url.toString());
}

void URLFrame::SetStatus(URLFrame::Status status)
{
    switch (status) {
    case SUCCESS:
        ui->status->setPixmap(QPixmap(":/pixmap/success.png"));
        break;
    case FAIL:
        ui->status->setPixmap(QPixmap(":/pixmap/fail.png"));
        break;
    default:
        ui->status->setPixmap(QPixmap());
        break;
    }
}
