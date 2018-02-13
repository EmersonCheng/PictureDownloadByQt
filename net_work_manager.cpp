#include "net_work_manager.h"
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>
#include <QMimeDatabase>

NetWorkManager::NetWorkManager(QString download_dir_path, QObject *parent) :
    QObject(parent),
    download_dir(download_dir_path)
{
    manager = new QNetworkAccessManager(this);
    QDir().mkpath(download_dir);
}

void NetWorkManager::SetDownloadDir(QString path)
{
    download_dir = path;
}

void NetWorkManager::DownloadFile(QUrl _url, bool is_all_override)
{
    url = _url;

    QString fileName = url.fileName();
    if (fileName.isEmpty())
    {
        qDebug() << "no filename";
        emit FinishDownload(false,QString());
        return;
    }

    file = new QFile(download_dir + fileName);
    if (file->exists())
    {
        if(!is_all_override)
        {
            if (QMessageBox::question(NULL, tr("HTTP"),
                                      tr("There already exists a file called %1 in "
                                         "the current directory. Overwrite?").arg(fileName),
                                      QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
                    == QMessageBox::No)
            {
                qDebug() << "file no override";
                emit FinishDownload(false,QString());
                return;
            }
        }
        qDebug() << "file override";
        file->remove();
    }
    if (!file->open(QIODevice::WriteOnly))
    {
        QMessageBox::information(NULL, tr("HTTP"),
                                 tr("Unable to save the file %1: %2.")
                                 .arg(fileName).arg(file->errorString()));
        qDebug() << "file open fail";
        delete file;
        file = 0;
        emit FinishDownload(false,QString());
        return;
    }

    // schedule the request
    httpRequestAborted = false;
    StartRequest();
}

void NetWorkManager::AbortDownload()
{
    httpRequestAborted = true;
}

void NetWorkManager::HttpFinish()
{
    qDebug() << "HttpFinish" << reply->header(QNetworkRequest::ContentTypeHeader);

    if (httpRequestAborted)
    {
        qDebug() << "Abort Download";
        if (file)
        {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        reply = 0;
        emit FinishDownload(false,QString());
        return;
    }

    if(file)
    {
        file->flush();
        file->close();

        if (reply->error())
        {
            file->remove();

            qDebug() << "reply error" << reply->errorString();
            //        QMessageBox::information(NULL, tr("HTTP"),
            //                                 tr("Download failed: %1.")
            //                                 .arg(reply->errorString()));

            reply->deleteLater();
            reply = 0;
            delete file;
            file = 0;
            emit FinishDownload(false,QString());
        }
        else
        {
            QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
            if (!redirectionTarget.isNull())
            {
                QUrl newUrl = url.resolved(redirectionTarget.toUrl());
                if (QMessageBox::question(NULL, tr("HTTP"),
                                          tr("Redirect to %1 ?").arg(newUrl.toString()),
                                          QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
                {
                    qDebug() << tr("Redirect to %1 ?").arg(newUrl.toString());
                    url = newUrl;
                    reply->deleteLater();
                    reply = 0;
                    file->open(QIODevice::WriteOnly);
                    file->resize(0);
                    StartRequest();
                    return;
                }
                else
                {
                    qDebug() << tr("No Redirect to %1 ?").arg(newUrl.toString());
                    reply->deleteLater();
                    reply = 0;
                    delete file;
                    file = 0;
                    emit FinishDownload(false,QString());
                }
            }
            else
            {
                QMimeType type = QMimeDatabase().mimeTypeForName(reply->header(QNetworkRequest::ContentTypeHeader).toString());
                QFileInfo file_info(file->fileName());
                QString suffix = file_info.suffix();
                if(suffix.isEmpty())
                {
                    file->rename(file->fileName() + "." + type.preferredSuffix());
                }
                else
                {
                    if(!type.suffixes().contains(suffix))
                    {
                        qDebug() << "file need rename";
                        QString new_name = file->fileName().replace(file_info.completeSuffix(),type.preferredSuffix());
                        file->rename(new_name);
                    }
                }

                reply->deleteLater();
                reply = 0;
                QString file_name = file->fileName();
                delete file;
                file = 0;
                emit FinishDownload(true,file_name);
            }
        }
    }
}

void NetWorkManager::HttpReadyRead()
{
//    qDebug() << "HttpReadyRead";
    if (file)
        file->write(reply->readAll());
}

void NetWorkManager::StartRequest()
{
    qDebug() << "StartRequest";
    reply = manager->get(QNetworkRequest(url));
    connect(reply,&QNetworkReply::finished,this,&NetWorkManager::HttpFinish);
    connect(reply,&QNetworkReply::readyRead,this,&NetWorkManager::HttpReadyRead);
}
