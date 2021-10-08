#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QNetworkRequest>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

private:
    bool GetLogInQr();
    void SetCookie(QNetworkRequest &req);
    void UpdateCookie(const QNetworkReply &rep);

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *manager;
    QNetworkCookieJar *cookieJar;
};
#endif // MAINWINDOW_H
