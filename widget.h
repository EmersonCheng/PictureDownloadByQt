#ifndef WIDGET_H
#define WIDGET_H

#include <QSettings>
#include <QWidget>
#include "net_work_manager.h"
#include "urlframe.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void FinishDownload(bool is_success, QString file_path);

protected:
    void closeEvent(QCloseEvent *);

private:
    void ReadSetting();
    void SelectDownloadPath();
    void AddUrlFrame();
    void Download();
    void AbortDownload();

    Ui::Widget *ui;
    QSettings *setting;
    NetWorkManager *network_manager;
    QString download_dir;
    QList<URLFrame*> url_frame_list;
    int current_download_file;
};

#endif // WIDGET_H
