#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QEventLoop>
#include <QTimer>
#include <QPixmap>
#include <QImage>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    cookieJar = new QNetworkCookieJar(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    // 扫码登录

    // 获取二维码

    if (GetLogInQr())
    {
        ;
    }
    else
    {
        qDebug("Get Login QrCode error");
        return;
    }
    // 扫描扫码结果
}


bool MainWindow::GetLogInQr()
{
    QNetworkRequest req;

    req.setUrl(QUrl("https://login.10086.cn/genqr.htm"));

    req.setRawHeader("Referer", "https://login.10086.cn/");


    QEventLoop loop;
    QTimer t;
    t.setInterval(10000);
    connect(&t, &QTimer::timeout,
            &loop, &QEventLoop::quit);

    // SetCookie(req);
    t.start();
    QNetworkReply *rep = manager->get(req);
    connect(rep, &QNetworkReply::finished,
            &loop, &QEventLoop::quit);
    loop.exec();

    if (t.isActive())
    {
        UpdateCookie(*rep);
        QByteArray a = rep->readAll();
        QPixmap map = QPixmap::fromImage(QImage::fromData(a));

        map.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        ui->label->setScaledContents(true);
        ui->label->setPixmap(map);
        rep->deleteLater();
        return true;
    }
    rep->deleteLater();
    return false;
}

void MainWindow::SetCookie(QNetworkRequest &req)
{
    ;
}

void MainWindow::UpdateCookie(const QNetworkReply &rep)
{
    QList<QByteArray> hs = rep.rawHeaderList();
    QVariant variantCookies = rep.header(QNetworkRequest::SetCookieHeader);
    QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(variantCookies);
    for (auto c : cookies)
    {
        qDebug("cookie : %s", c.toRawForm().data());
        cookieJar->insertCookie(c);
    }
}
