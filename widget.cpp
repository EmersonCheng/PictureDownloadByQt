#include "widget.h"
#include "ui_widget.h"

#include <QFileDialog>
#include <QFormLayout>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QProcess>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    setting = new QSettings(qApp->applicationDirPath() + "/setting.ini",QSettings::IniFormat,this);
    ReadSetting();

    network_manager = new NetWorkManager(QDir::tempPath()+"/pic_download/",this);
    ui->cancle_btn->setEnabled(false);

    connect(network_manager,&NetWorkManager::FinishDownload,this,&Widget::FinishDownload);
    connect(ui->down_path_btn,&QPushButton::clicked,this,&Widget::SelectDownloadPath);
    connect(ui->add_btn,&QPushButton::clicked,this,&Widget::AddUrlFrame);
    connect(ui->down_btn,&QPushButton::clicked,this,&Widget::Download);
    connect(ui->cancle_btn,&QPushButton::clicked,this,&Widget::AbortDownload);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::FinishDownload(bool is_success, QString file_path)
{
    if(is_success)
    {
        url_frame_list.at(current_download_file)->SetStatus(URLFrame::SUCCESS);
        QFile::copy(file_path,download_dir + QFileInfo(file_path).fileName());
        QFile::remove(file_path);
    }
    else
        url_frame_list.at(current_download_file)->SetStatus(URLFrame::FAIL);
    current_download_file++;
    if(current_download_file < url_frame_list.size())
        network_manager->DownloadFile(url_frame_list.at(current_download_file)->GetUrl(),true);
    else
    {
        ui->down_btn->setEnabled(true);
        ui->cancle_btn->setEnabled(false);

        QMessageBox::information(this,tr("下载完成"),tr("全部下载完成"));
    }
}

void Widget::closeEvent(QCloseEvent *)
{
    qDebug() << "close";
    setting->setValue("download_dir",download_dir);
    setting->beginWriteArray("url");
    for(int i = 0; i < url_frame_list.size(); i++)
    {
        QUrl tmp = url_frame_list.at(i)->GetUrl();
        if(!tmp.isEmpty())
        {
            static int j = 0;
            setting->setArrayIndex(j);
            setting->setValue("url",tmp);
            j++;
        }
    }
    setting->endArray();
    setting->sync();   
}

void Widget::ReadSetting()
{
    download_dir = setting->value("download_dir",QDir::home().path() + "/Desktop").toString();
    ui->down_path_lineedit->setText(QDir::toNativeSeparators(download_dir));

    int size = setting->beginReadArray("url");
    for(int i = 0; i < size; i++)
    {
        setting->setArrayIndex(i);
        AddUrlFrame();
        url_frame_list.last()->SetUrl(setting->value("url").toUrl());
    }
    if(size == 0)
        ui->down_btn->setEnabled(false);
    setting->endArray();
}

void Widget::SelectDownloadPath()
{
    QString tmp = QFileDialog::getExistingDirectory(this,tr("选择下载路径"),ui->down_path_lineedit->text());
    if(!tmp.isEmpty())
        ui->down_path_lineedit->setText(QDir::toNativeSeparators(tmp));
}

void Widget::AddUrlFrame()
{
    static int count = 0;
    static QFormLayout *frame_layout = new QFormLayout;
    if(count == 0)
    {
        ui->scrollAreaWidgetContents->setLayout(frame_layout);
        ui->down_btn->setEnabled(true);
    }

    count++;
    QLabel *label = new QLabel(QString::number(count)+".",this);
    label->setFixedHeight(24);
    label->setLineWidth(1);
    URLFrame *url_frame = new URLFrame(this);
    url_frame_list.append(url_frame);

    frame_layout->addRow(label,url_frame);
}

void Widget::Download()
{
    QString path = ui->down_path_lineedit->text();
    if(path.isEmpty())
    {
        QMessageBox::warning(this,tr("警告"),tr("下载路径为空"),QMessageBox::Ok);
        return;
    }
    if(!QDir(path).exists())
    {
        QMessageBox::warning(this,tr("警告"),tr("下载路径不存在"),QMessageBox::Ok);
        return;
    }
    download_dir = QDir(path).path() + "/";

    ui->down_btn->setEnabled(false);
    ui->cancle_btn->setEnabled(true);
    current_download_file = 0;
    network_manager->DownloadFile(url_frame_list.at(current_download_file)->GetUrl(),true);
}

void Widget::AbortDownload()
{
    network_manager->AbortDownload();
    ui->cancle_btn->setEnabled(false);
    ui->down_btn->setEnabled(true);
    for(; current_download_file < url_frame_list.size(); current_download_file++)
    {
        url_frame_list.at(current_download_file)->SetStatus(URLFrame::FAIL);
    }
}
