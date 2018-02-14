#include "widget.h"
#include <QApplication>
#include <QDateTime>
#include <QString>
#include <QFile>
#include <QTextStream>

#define LOG_PATH qApp->applicationDirPath()+"/log.txt"

void MessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString out;
    out += QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss ");
    switch (type) {
    case QtDebugMsg:
        out += "Debug: ";
        break;
    case QtInfoMsg:
        out += "Info: ";
        break;
    case QtWarningMsg:
        out += "Warning: ";
        break;
    case QtCriticalMsg:
        out += "Critical: ";
        break;
    case QtFatalMsg:
        out += "Fatal: ";
        break;
        abort();
    }
    out += QString("%1  (%2,%3)").arg(msg).arg(context.file).arg(context.line);

    QTextStream str(stderr,QIODevice::WriteOnly);
    str << out << endl;

    static QFile file(LOG_PATH);
    static QTextStream text_stream(&file);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    text_stream << out << "\r\n";
    file.close();

}

void OpenLogOutput()
{
    QFile file(LOG_PATH);
    QTextStream text_stream(&file);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    text_stream << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ") << "APPLICATION OPEN\r\n";
    file.close();
    qInstallMessageHandler(MessageOutput);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("微软雅黑",14));
    OpenLogOutput();

    Widget w;
    w.show();

    return a.exec();
}
