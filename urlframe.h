#ifndef URLFRAME_H
#define URLFRAME_H

#include <QFrame>
#include <QUrl>

namespace Ui {
class URLFrame;
}

class URLFrame : public QFrame
{
    Q_OBJECT

public:
    enum Status{SUCCESS,FAIL};

    explicit URLFrame(QWidget *parent = 0);
    ~URLFrame();
    QUrl GetUrl();
    void SetUrl(QUrl url);
    void SetStatus(Status status);

private:
    Ui::URLFrame *ui;
};

#endif // URLFRAME_H
