#ifndef NET_WORK_MANAGER_H
#define NET_WORK_MANAGER_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QObject>

class NetWorkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetWorkManager(QString download_dir_path , QObject *parent = 0);
    void SetDownloadDir(QString path);
    void DownloadFile(QUrl _url, bool is_all_overwrite);
    void AbortDownload();

signals:
    void FinishDownload(bool is_success, QString file_path);

private slots:
    void HttpFinish();
    void HttpReadyRead();

private:
    void StartRequest();

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *file;
    QUrl url;
    QString download_dir;
    bool httpRequestAborted;

};

#endif // NET_WORK_MANAGER_H
