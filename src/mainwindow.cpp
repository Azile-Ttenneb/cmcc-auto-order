#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QEventLoop>
#include <QTimer>
#include <QPixmap>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>

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
    if (GetLogInResult())
    {
        ;
    }
    else
    {
        qDebug("login failed");
    }

    // GET /i/v1/auth/getArtifact

    // 登录个人信息获取
    // GET /i/v1/auth/loginfo?_=1633604378637

    // 购物车
    // GET /i/v1/cust/cartNumber/13853162732?userId=13853162732&userType=0&_=1633604379014

    // 话费查询
    // GET /i/v1/fee/real/13853162732?_=1633604379092


    // 积分查询
    // GET /i/v1/point/sum/13853162732?_=1633604379093

    // 套餐余量
    // GET /i/v1/fee/planbal/13853162732?_=1633604379095

    // 地址
    // GET /i/v1/res/numarea/13853162732?_=1633604379095

    // 购物车物品
    // GET /i/v1/cust/cartInfo/13853162732?userId=13853162732&userType=0&_=1633605111420

    // 单点登录检查
    // GET /SSOCheck.action?channelID=12002&backUrl=https://shop.10086.cn/goods/100_100_1085836_1075017.html
    // GET /sso/getartifact.php?artifact=-1&backUrl=https%3A%2F%2Fshop.10086.cn%2Fgoods%2F100_100_1085836_1075017.html

    // 查询余量
    // GET /ajax/detail/getstock.json

    // 访问记录
    // GET /ajax/detail/addfootprint.json

    // 购买
    // POST /ajax/buy/buy.json

    // 用户信息
    // GET /ajax/user/userinfo.json

    // 提交订单
    // POST /ajax/submitorder/addorder.json

    // 跳转支付界面
    // GET /order/topay.php?orderId=416336063806541638
}


bool MainWindow::GetLogInQr()
{
    QString url = "https://login.10086.cn/genqr.htm";
    QMap<QByteArray, QByteArray> headers;
    headers["Referer"] = "https://login.10086.cn/";
    QByteArray a ;
    if (GetMethod(url, headers, 10000, a))
    {
        lgToken = GetCookieValue("lgToken");
        QPixmap map = QPixmap::fromImage(QImage::fromData(a));

        map.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        ui->label->setScaledContents(true);
        ui->label->setPixmap(map);
        return true;
    }

    return false;
}

bool MainWindow::GetLogInResult()
{
    for (int count = 0; count < 30; count++)
    {
        QNetworkRequest req;

        QString url = "https://login.10086.cn/chkqr.htm";
        QMap<QByteArray, QByteArray> headers;
        headers["Referer"] = "https://login.10086.cn/";

        QByteArray data = "lgToken=";
        data += lgToken;
        data += "&targetChannelID=12003&backUrl=http%3A%2F%2Fshop.10086.cn%2Fi%2F";
        QByteArray a ;
        if (PostMethod(url, headers, data, 1000, a))
        {
            if (a.length() > 0)  // 已扫码，登录成功
            {
                QJsonObject obj = QJsonDocument::fromJson(a).object();
                if (!obj.isEmpty() && obj.contains("artifact"))
                {
                    artifact = obj["artifact"].toString().toUtf8();
                }
                return true;
            }
        }
        // sleep(1000)

    }
    return false;
}

bool MainWindow::GetArtifact(){return false;}
bool MainWindow::GetLoginInfo(){return false;}
bool MainWindow::GetCartInfo(){return false;}
bool MainWindow::GetFee(){return false;}
bool MainWindow::GetPoints(){return false;}
bool MainWindow::GetAddress(){return false;}
bool MainWindow::GetCartGoods(){return false;}
bool MainWindow::CheckSSO(){return false;}
bool MainWindow::GetStock(){return false;}
bool MainWindow::AddFootPrint(){return false;}
bool MainWindow::Buy(){return false;}
bool MainWindow::GetUserInfo(){return false;}
bool MainWindow::SummitOrder(){return false;}
bool MainWindow::PayPage(){return false;}
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

QByteArray MainWindow::GetCookieValue(const QByteArray &k)
{
    QByteArray ret;

    return ret;
}

bool MainWindow::GetMethod(const QString &url, const QMap<QByteArray, QByteArray> &headers, int timeout, QByteArray &r)
{
    QNetworkRequest req;

    req.setUrl(url);

    for (auto i = headers.begin(); i != headers.end(); i++)
    {
        req.setRawHeader(i.key(), i.value());
    }

    if (timeout <= 0)
    {
        timeout = 5000;
    }

    QEventLoop loop;
    QTimer t;
    t.setInterval(timeout);
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
        r = rep->readAll();
        rep->deleteLater();
        return true;
    }
    rep->deleteLater();
    return false;
}

bool MainWindow::PostMethod(const QString &url, const QMap<QByteArray, QByteArray> &headers, const QByteArray &body, int timeout, QByteArray &r)
{
    QNetworkRequest req;

    req.setUrl(url);

    for (auto i = headers.begin(); i != headers.end(); i++)
        req.setRawHeader(i.key(), i.value());

    if (timeout <= 0)
        timeout = 5000;
    QEventLoop loop;
    QTimer t;
    t.setInterval(1000);
    connect(&t, &QTimer::timeout,
            &loop, &QEventLoop::quit);

    // SetCookie(req);
    t.start();
    QNetworkReply *rep = manager->post(req, body);
    connect(rep, &QNetworkReply::finished,
            &loop, &QEventLoop::quit);
    loop.exec();

    if (t.isActive())
    {
        UpdateCookie(*rep);
        r = rep->readAll();
        rep->deleteLater();
        return true;
    }
    rep->deleteLater();
    return false;
}

void MainWindow::on_pushButton_2_clicked()
{
    // 检测扫码结果
    // aaa
}

void MainWindow::on_pushButton_3_clicked()
{
    // 获取Artifact
    // aaa
}

void MainWindow::on_pushButton_4_clicked()
{
    // 获取个人登录信息
    // aaa
}

void MainWindow::on_pushButton_5_clicked()
{
    // 获取个人购物车信息
    // aaa
}

void MainWindow::on_pushButton_6_clicked()
{
    // 查询话费余额
    // aaa
}

void MainWindow::on_pushButton_7_clicked()
{
    // 查询积分
    // aaa
}

void MainWindow::on_pushButton_8_clicked()
{
    // 查询套餐余量
    // aaa
}

void MainWindow::on_pushButton_9_clicked()
{
    // 查询发货地址
    // aaa
}

void MainWindow::on_pushButton_10_clicked()
{
    // 查询购物车物品
    // aaa
}

void MainWindow::on_pushButton_11_clicked()
{
    // 单点登录检查
    // aaa
}

void MainWindow::on_pushButton_12_clicked()
{
    // 查询商品余量
    // aaa
}

void MainWindow::on_pushButton_13_clicked()
{
    // 添加访问记录
    // aaa
}

void MainWindow::on_pushButton_14_clicked()
{
    // 购买
    // aaa
}

void MainWindow::on_pushButton_15_clicked()
{
    // 用户信息查询
    // aaa
}

void MainWindow::on_pushButton_16_clicked()
{
    // 提交订单
    // aaa
}

void MainWindow::on_pushButton_17_clicked()
{
    // 跳转支付界面
    // aaa
}
