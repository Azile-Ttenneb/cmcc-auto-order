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

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

private:
    bool GetLogInQr();
    bool GetLogInResult();
    bool GetArtifact();
    bool GetLoginInfo();
    bool GetCartInfo();
    bool GetFee();
    bool GetPoints();
    bool GetPlanBal();
    bool GetAddress();
    bool GetCartGoods();
    bool CheckSSO();
    bool GetStock();
    bool AddFootPrint();
    bool Buy();
    bool GetUserInfo();
    bool SummitOrder();
    bool PayPage();

    void SetCookie(QNetworkRequest &req);
    void UpdateCookie(const QNetworkReply &rep);
    bool GetMethod(const QString &url, const QMap<QByteArray, QByteArray> &headers, int timeout, QByteArray &r);
    bool PostMethod(const QString &url, const QMap<QByteArray, QByteArray> &headers, const QByteArray &body, int timeout, QByteArray &r);

    QByteArray GetCookieValue(const QString &url, const QByteArray &k);
    void Sleep(int time);
    QByteArray GetCurrentTimeStr();

private:
    Ui::MainWindow *ui;

    QNetworkAccessManager *manager;
    QNetworkCookieJar *cookieJar;

    QByteArray lgToken;
    QByteArray artifact;
    QByteArray mobileNo;

    QByteArray loginQr;
    QByteArray cartCode;
};
#endif // MAINWINDOW_H
