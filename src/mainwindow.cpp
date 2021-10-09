#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QEventLoop>
#include <QTimer>
#include <QPixmap>
#include <QImage>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include "logger.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    manager = new QNetworkAccessManager(this);
    cookieJar = new QNetworkCookieJar(this);
    manager->setCookieJar(cookieJar);
    mobileNo = "13853162732";
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
}


void MainWindow::on_pushButton_2_clicked()
{
    // 检测扫码结果
    if (GetLogInResult())
    {
        ;
    }
    else
    {
        qDebug("GetLogInResult failed");
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    // 获取Artifact
    if (GetArtifact())
    {
        ;
    }
    else
    {
        qDebug("GetArtifact failed");
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    // 获取个人登录信息
    if (GetLoginInfo())
    {
        ;
    }
    else
    {
        qDebug("GetLoginInfo failed");
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    // 获取个人购物车信息
    if (GetCartInfo())
    {
        ;
    }
    else
    {
        qDebug("GetCartInfo failed");
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    // 查询话费余额
    if (GetFee())
    {
        ;
    }
    else
    {
        qDebug("GetFee failed");
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    // 查询积分
    if (GetPoints())
    {
        ;
    }
    else
    {
        qDebug("GetPoints failed");
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    // 查询套餐余量
    if (GetPlanBal())
    {
        ;
    }
    else
    {
        qDebug("GetPlanBal failed");
    }
}

void MainWindow::on_pushButton_9_clicked()
{
    // 查询发货地址
    if (GetAddress())
    {
        ;
    }
    else
    {
        qDebug("GetAddress failed");
    }
}

void MainWindow::on_pushButton_10_clicked()
{
    // 查询购物车物品
    if (GetCartGoods())
    {
        ;
    }
    else
    {
        qDebug("GetCartGoods failed");
    }
}

void MainWindow::on_pushButton_11_clicked()
{
    // 单点登录检查
    if (CheckSSO())
    {
        ;
    }
    else
    {
        qDebug("CheckSSO failed");
    }
}

void MainWindow::on_pushButton_12_clicked()
{
    // 查询商品余量
    if (GetStock())
    {
        ;
    }
    else
    {
        qDebug("GetStock failed");
    }
}

void MainWindow::on_pushButton_13_clicked()
{
    // 添加访问记录
    if (AddFootPrint())
    {
        ;
    }
    else
    {
        qDebug("AddFootPrint failed");
    }
}

void MainWindow::on_pushButton_14_clicked()
{
    // 购买
    if (Buy())
    {
        ;
    }
    else
    {
        qDebug("Buy failed");
    }
}

void MainWindow::on_pushButton_15_clicked()
{
    // 用户信息查询
    if (GetUserInfo())
    {
        ;
    }
    else
    {
        qDebug("GetUserInfo failed");
    }
}

void MainWindow::on_pushButton_16_clicked()
{
    // 提交订单
    if (SummitOrder())
    {
        ;
    }
    else
    {
        qDebug("SummitOrder failed");
    }
}

void MainWindow::on_pushButton_17_clicked()
{
    // 跳转支付界面
    if (PayPage())
    {
        ;
    }
    else
    {
        qDebug("PayPage failed");
    }
}

bool MainWindow::GetLogInQr()
{
    QString url = "https://login.10086.cn/genqr.htm";
    QMap<QByteArray, QByteArray> headers;
    headers["Referer"] = "https://login.10086.cn/";
    QByteArray a ;
    if (GetMethod(url, headers, 10000, a))
    {
        lgToken = GetCookieValue(url, "lgToken");
        QPixmap map = QPixmap::fromImage(QImage::fromData(a));

        map.scaled(ui->label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        ui->label->setScaledContents(true);
        ui->label->setPixmap(map);
        qDebug("login qr loaded. lgToken %s", lgToken.data());
        return true;
    }

    qDebug("get login qr failed");
    return false;
}

bool MainWindow::GetLogInResult()
{
    QNetworkRequest req;

    QString url = "https://login.10086.cn/chkqr.htm";
    QMap<QByteArray, QByteArray> headers;
    headers["Referer"] = "https://login.10086.cn/";
    headers["Content-Type"] = "application/x-www-form-urlencoded; charset=UTF-8";

    QByteArray data = "lgToken=";
    data += lgToken;
    data += "&targetChannelID=12003&backUrl=http%3A%2F%2Fshop.10086.cn%2Fi%2F";
    QByteArray a ;
    if (PostMethod(url, headers, data, 1000, a))
    {
        if (a.length() > 0) // 已扫码，登录成功
        {
            QJsonObject obj = QJsonDocument::fromJson(a).object();
            if (!obj.isEmpty() && obj.contains("artifact"))
            {
                artifact = obj["artifact"].toString().toUtf8();
            }
            qDebug("login done. artifact %s", artifact.data());
            return true;
        }
    }

    qDebug("get login result failed");
    return false;
}

bool MainWindow::GetArtifact()
{
    // GET /i/v1/auth/getArtifact
    QString url = "https://shop.10086.cn/i/v1/auth/getArtifact?backUrl=http://shop.10086.cn/i/&artifact=";
    url += artifact;
    QMap<QByteArray, QByteArray> headers;
    headers["Referer"] = "https://login.10086.cn/";
    QByteArray a ;
    if (GetMethod(url, headers, 10000, a))
    {
        qDebug("get artifact succeed.");
        return true;
    }
    qDebug("get artifact failed.");
    return false;
}

bool MainWindow::GetLoginInfo()
{
    // 登录个人信息获取
    // GET /i/v1/auth/loginfo?_=1633604378637
    QString url = "https://shop.10086.cn/i/v1/auth/loginfo?_=";
    url.append(GetCurrentTimeStr());

    QMap<QByteArray, QByteArray> headers;
    // headers["Referer"] = "https://shop.10086.cn/i/?welcome=1633604378859";
    headers["Referer"] = "https://shop.10086.cn/";

    QByteArray a ;
    if (GetMethod(url, headers, 10000, a))
    {
        QJsonObject obj = QJsonDocument::fromJson(a).object();
        if (obj.contains("data"))
        {
            QJsonObject o2 = obj["data"].toObject();
            if (o2.contains("loginValue"))
            {
                mobileNo = o2["loginValue"].toString().toUtf8();
                qDebug("get login info succeed. loginValue %s", mobileNo.data());
                return true;
            }
        }
    }

    qDebug("get login info failed.");
    return false;
}

bool MainWindow::GetCartInfo()
{
    // 购物车
    // GET /i/v1/cust/cartNumber/13853162732?userId=13853162732&userType=0&_=1633604379014
    QString url = "https://shop.10086.cn/i/v1/cust/cartNumber/";
    url.append(mobileNo).append("?userId=").append(mobileNo).append("&userType=0&_=");
    url.append(GetCurrentTimeStr());

    QMap<QByteArray, QByteArray> headers;
    // headers["Referer"] = "https://shop.10086.cn/i/?welcome=1633604378859";
    headers["Referer"] = "https://shop.10086.cn/";

    QByteArray a ;
    if (GetMethod(url, headers, 10000, a))
    {
        qDebug("get cart info succeed. data %s", a.data());
        return true;
    }

    qDebug("get cart info failed.");
    return false;
}

bool MainWindow::GetFee()
{
    // 话费查询
    // GET /i/v1/fee/real/13853162732?_=1633604379092
    QString url = "https://shop.10086.cn/i/v1/fee/real/";
    url.append(mobileNo).append("?_=");
    url.append(GetCurrentTimeStr());

    QMap<QByteArray, QByteArray> headers;
    // headers["Referer"] = "https://shop.10086.cn/i/?welcome=1633604378859";
    headers["Referer"] = "https://shop.10086.cn/";

    QByteArray a ;
    if (GetMethod(url, headers, 10000, a))
    {
        qDebug("get fee info succeed. data %s", a.data());
        return true;
    }

    qDebug("get fee info failed. data %s", a.data());
    return false;
}

bool MainWindow::GetPoints()
{
    // 积分查询
    // GET /i/v1/point/sum/13853162732?_=1633604379093
    QString url = "https://shop.10086.cn/i/v1/point/sum/";
    url.append(mobileNo).append("?_=");
    url.append(GetCurrentTimeStr());

    QMap<QByteArray, QByteArray> headers;
    // headers["Referer"] = "https://shop.10086.cn/i/?welcome=1633604378859";
    headers["Referer"] = "https://shop.10086.cn/";

    QByteArray a ;
    if (GetMethod(url, headers, 10000, a))
    {
        qDebug("get points info succeed. data %s", a.data());
        return true;
    }

    qDebug("get points info failed. data %s", a.data());
    return false;
}

bool MainWindow::GetPlanBal()
{
    // 套餐余量
    // GET /i/v1/fee/planbal/13853162732?_=1633604379095
    QString url = "https://shop.10086.cn/i/v1/fee/planbal/";
    url.append(mobileNo).append("?_=");
    url.append(GetCurrentTimeStr());

    QMap<QByteArray, QByteArray> headers;
    // headers["Referer"] = "https://shop.10086.cn/i/?welcome=1633604378859";
    headers["Referer"] = "https://shop.10086.cn/";

    QByteArray a ;
    if (GetMethod(url, headers, 10000, a))
    {
        qDebug("get plan balance info succeed. data %s", a.data());
        return true;
    }

    qDebug("get plan balance info failed. data %s", a.data());
    return false;
}

bool MainWindow::GetAddress()
{
    // 地址
    // GET /i/v1/cust/recaddr/13853162732?_=1633606653055
    QString url = "https://shop.10086.cn/i/v1/cust/recaddr/";
    url.append(mobileNo).append("?_=");
    url.append(GetCurrentTimeStr());

    QMap<QByteArray, QByteArray> headers;
    // headers["Referer"] = "https://shop.10086.cn/i/?welcome=1633604378859";
    headers["Referer"] = "https://shop.10086.cn/";

    QByteArray a ;
    if (GetMethod(url, headers, 10000, a))
    {
        qDebug("get address info succeed. data %s", a.data());
        return true;
    }

    qDebug("get address info failed. data %s", a.data());
    return false;
}

bool MainWindow::GetCartGoods()
{
    // 购物车物品
    // GET /i/v1/cust/cartInfo/13853162732?userId=13853162732&userType=0&_=1633605111420
    QString url = "https://shop.10086.cn/i/v1/cust/cartInfo/";
    url.append(mobileNo).append("?userId=").append(mobileNo).append("&userType=0&_=");
    url.append(GetCurrentTimeStr());

    QMap<QByteArray, QByteArray> headers;
    // headers["Referer"] = "https://shop.10086.cn/i/?welcome=1633604378859";
    headers["Referer"] = "https://shop.10086.cn/";

    QByteArray a ;
    if (GetMethod(url, headers, 10000, a))
    {
        qDebug("get card goods info succeed. data %s", a.data());
        return true;
    }

    qDebug("get card goods info failed. data %s", a.data());
    return false;
}

bool MainWindow::CheckSSO()
{
    // 单点登录检查
    // GET /SSOCheck.action?channelID=12002&backUrl=https://shop.10086.cn/goods/100_100_1085836_1075017.html
    QString url = "https://login.10086.cn/SSOCheck.action?channelID=12002&backUrl=https://shop.10086.cn/goods/100_100_1085836_1075017.html";


    QMap<QByteArray, QByteArray> headers;
    // headers["Referer"] = "https://shop.10086.cn/i/?welcome=1633604378859";
    headers["Referer"] = "https://shop.10086.cn/";

    QByteArray a ;
    if (!GetMethod(url, headers, 10000, a))
    {
        qDebug("sso check failed. data %s", a.data());
        return false;
    }
    qDebug("sso check succeed. data %s", a.data());

    // GET /sso/getartifact.php?artifact=-1&backUrl=https%3A%2F%2Fshop.10086.cn%2Fgoods%2F100_100_1085836_1075017.html
    url = "https://shop.10086.cn/sso/getartifact.php?artifact=-1&backUrl=https%3A%2F%2Fshop.10086.cn%2Fgoods%2F100_100_1085836_1075017.html";
    a.clear();
    if (!GetMethod(url, headers, 10000, a))
    {
        qDebug("sso check failed. data %s", a.data());
        return false;
    }
    qDebug("sso get artifact succeed. data %s", a.data());

    return true;
}

bool MainWindow::GetStock()
{
    // 查询余量
    // GET /ajax/detail/getstock.json
    QString url = "https://shop.10086.cn/ajax/detail/getstock.json?0UmtWaqB=5ecq_xcfhhN29mTJaSwtzl7.FeLxKDEfiCFSPvVMgU0Ze2ilWKsoLz.qWi33dNfBFBwGIwLSNyEtnneqpl9Vo._2FjWwr62VLemUJN8fNS6foYkkXyC2W8v0EN7MaAk0QnWkHYThR0VXOvBpOMTLh05FX3APFxQDyxLLTikV56sYdDALY0IItH7ueZn1tk8y_A0AUxFI9ZVCd9OKWj2wHY3GHEjRkvu1Usm4lVdO354XnHxX1StMsJo.c8CpS7TyK3WYyDADekxvNfUNLlxewRQQzD_6CESrVpmeuHpF2WFql175yBr6IbEd0b9t.iU_mmB6AmddvxUudcQ5UNUjXMJezsv8PpuDmHAgHvbb4suG&ef702PNB=4aJlAmMWLUYjN0lCu7c1rYVDSPLvqsi07I59vJRf5XTkEeOZGIEnEU4pyDfShJpI0_yxjv7WeERovRbTRhdOcYOwfftkBn4EHxm4jmL1QVpW7PiCXtHqmarHr3E3k7Nq85gHyDkE8DpiLpLcKVlVTtkJ7RIEGGU.NiConB424A8LsHqypRDj60A4PGHmMxuaZwB8jNsmWnZuHNEZDcqTMjq";


    QMap<QByteArray, QByteArray> headers;
    // headers["Referer"] = "https://shop.10086.cn/i/?welcome=1633604378859";
    headers["Referer"] = "https://shop.10086.cn/";

    QByteArray a ;
    if (GetMethod(url, headers, 10000, a))
    {
        qDebug("get stock info succeed. data %s", a.data());
        return true;
    }

    qDebug("get stock info failed. data %s", a.data());
    return false;
}

bool MainWindow::AddFootPrint()
{
    // 访问记录
    // GET /ajax/detail/addfootprint.json
    QString url = "https://shop.10086.cn/ajax/detail/addfootprint.json?0UmtWaqB=55g2NUVvklQUzwM_epakUS40Z7sbb6hkXHvuE3_SmEo33mF2GRabb8EwyeHS8.imbISaH6OUXHDET6KdDtxh2JM_cLeU.FTUrHxx5c7ZA6Mq2Vt7HjvS19ZzJTRJUF7ATbuk3x9RbfNiSG34I9gRImLyR5Oq50sq0HsygbLb6XvnHug1dtgYUl9yeidIScyBXdfwWVNw3t9.LiqiyZu0pPsYUj1gl8yw_ed_Os2W7b65icqx5gpCZwhzVYZ.zQto8bqGgBmFXAKWBaXE7.jbEn5eEBgeFIcVjXIvLX2KSnK5KtJ.AqnG9zj1lkG6WGf1CmyLGYLGoLTEwOmQfqxMnqtV3ukhiZGb8MgYHSrYXYZA&ef702PNB=4ZZt82uYOfdxx1FmiuIRbIiw2JxEzaaGMWIgJLweQAyGHP._3vq_ZXcJvm40N5y9qCmj98E4bdCxWZuc2JU0ky6uMPYf8ryjji9L9XPNCfk.iGM1RA10o_JCdP9lHxX.mdm41ZNvzCmzeijWTiLxRCG";

    QMap<QByteArray, QByteArray> headers;
    // headers["Referer"] = "https://shop.10086.cn/i/?welcome=1633604378859";
    headers["Referer"] = "https://shop.10086.cn/";

    QByteArray a ;
    if (GetMethod(url, headers, 10000, a))
    {
        qDebug("get card goods info succeed. data %s", a.data());
        return true;
    }

    return false;
}

bool MainWindow::Buy()
{
    // 购买
    // POST /ajax/buy/buy.json
    QString url = "https://shop.10086.cn/ajax/buy/buy.json?0UmtWaqB=5gjM.iIw8y8B6rbUmwiGV_oMvkudusblE0LVEk6PylBULePsPCjlKacNpU.gLzg_hJI6cWPv46wsbZpDBrSaiVuo2ajpIqD2JXX054LjMQp2igOynirBjcoHyW3XZ.b70R8ghoW52b_qY9K2zHt7graFMU0.7bq6m2ECXT1E4xifoFhTjS7IFf5VnAsDdhbLzhNPGLpqUdWQyEjzpc5zS57juOb5uwya2G9oxsM8VYjvtn4t9ML2RRVqo3PEfoe.9aTB3_odyblWKcA2FPJW1.uGf_bj7WoJ1LJArwf5ko9b9oOrhEsiq6ptVXJrA8CbNUKoV2ACIjkkpJGklM.ladflAJgbmSSrQcOzOh.4HE9tmqd9mozkE9nKI6Yfkm0eua1qBa_NdxZ7GcvRN_N8HZG";

    QMap<QByteArray, QByteArray> headers;
    // headers["Referer"] = "https://shop.10086.cn/i/?welcome=1633604378859";
    headers["Referer"] = "https://shop.10086.cn/";
    headers["Content-Type"] = "application/x-www-form-urlencoded; charset=UTF-8";

    QByteArray data = "sku%5B0%5D%5BModelId%5D=1075007&sku%5B0%5D%5BGoodsId%5D=1085836&sku%5B0%5D%5BNum%5D=1&sku%5B0%5D%5BGoodsType%5D=70000&sku%5B0%5D%5BItemFrom%5D=PC_CUSTOM_617154_832597_2_1085836&sku%5B0%5D%5BChannel%5D=1&sku%5B0%5D%5BProvinceId%5D=531&sku%5B0%5D%5BCityId%5D=531";

    QByteArray a ;
    if (PostMethod(url, headers, data, 1000, a))
    {
        if (a.length() > 0) // 已扫码，登录成功
        {
            QJsonObject obj = QJsonDocument::fromJson(a).object();
            if (!obj.isEmpty() && obj.contains("code"))
            {
                if (obj["code"].toInt() == 0)
                {
                    if (obj.contains("data"))
                    {
                        QJsonObject o2 = obj["data"].toObject();
                        if (o2.contains("to_url"))
                        {
                            QString tourl = o2["to_url"].toString();
                            cartCode = tourl.mid(tourl.lastIndexOf('=')+1).toUtf8();
                        }
                    }
                }
            }
            qDebug("buy done. result %s", a.data());
            return true;
        }
    }

    return false;
}

bool MainWindow::GetUserInfo()
{
    // 用户信息
    // GET /ajax/user/userinfo.json
    QString url = "https://shop.10086.cn/ajax/user/userinfo.json?0UmtWaqB=5ecq_xcfhhN29mTJaSwtzl7.FeLxKDEfiCFSPvVMgU0Ze2ilWKsoLz.qWi33dNfBFBwGIwLSNyEtnneqpl9Vo._2FjWwr62VLemUJN8fNS6foYkkXyC2W8v0EN7MaAk0QnWkHYThR0VXOvBpOMTLh05FX3APFxQDyxLLTikV56sYdDALY0IItH7ueZn1tk8y_A0AUxFI9ZVCd9OKWj2wHY3GHEjRkvu1Usm4lVdO354XnHxX1StMsJo.c8CpS7TyK3WYyDADekxvNfUNLlxewRQQzD_6CESrVpmeuHpF2WFql175yBr6IbEd0b9t.iU_mmB6AmddvxUudcQ5UNUjXMJezsv8PpuDmHAgHvbb4suG&ef702PNB=4aJlAmMWLUYjN0lCu7c1rYVDSPLvqsi07I59vJRf5XTkEeOZGIEnEU4pyDfShJpI0_yxjv7WeERovRbTRhdOcYOwfftkBn4EHxm4jmL1QVpW7PiCXtHqmarHr3E3k7Nq85gHyDkE8DpiLpLcKVlVTtkJ7RIEGGU.NiConB424A8LsHqypRDj60A4PGHmMxuaZwB8jNsmWnZuHNEZDcqTMjq";
    url.append(mobileNo).append("?userId=").append(mobileNo).append("&userType=0&_=");
    url.append(GetCurrentTimeStr());

    QMap<QByteArray, QByteArray> headers;
    // headers["Referer"] = "https://shop.10086.cn/i/?welcome=1633604378859";
    headers["Referer"] = "https://shop.10086.cn/";

    QByteArray a ;
    if (GetMethod(url, headers, 10000, a))
    {
        qDebug("get user info succeed. data %s", a.data());
        return true;
    }

    return false;
}

bool MainWindow::SummitOrder()
{
    // 提交订单
    // POST /ajax/submitorder/addorder.json
    QString url = "https://shop.10086.cn/ajax/submitorder/addorder.json?0UmtWaqB=5FzBOK22tjGm90AnXCqi.n9EQnZuLX_CjIRnRlgHj.zutEEHKfA_gUZSX3M6Oqv1CSwtTG1Usu6uRSWP5Sq.UeFHpKrekJ3VJS5PWL3N1fyxUZSpxXs_SvIMfMvFNtfsntGN72AMqTK8Dxgi1txEkK5SeJ9ttjZFeiuhOCDBncUhRZ0bbgYnNYVUkjyLRWMsZtnR8SsIMPBg3gr38.T.hfFnwbBJQAFZhQYXfdgQyCH9yjQYy0ITlywtwf5QqpOjJljqt8KqcWZ4PEt0j0sL9cxWLINderMMXaq5xqkiNguUyEis.HIXHEj2FXg_hN3Ylk1jzibV2IBDMw_DvulX8heGeECIBNdvHwQUr95qwdPqqAkc3FF25QjlXyETTkDotEx0ffTpmkrobhsP5_3.FJG";

    QMap<QByteArray, QByteArray> headers;
    // headers["Referer"] = "https://shop.10086.cn/i/?welcome=1633604378859";
    headers["Referer"] = "https://shop.10086.cn/";
    headers["Content-Type"] = "application/x-www-form-urlencoded; charset=UTF-8";

    QByteArray data = "address_id=1633235039716R22&invoice_id=personal&pay_type=1&coupon_type=&ticket_no=&eticket_number_mall=&cart_code=";
    data.append(cartCode).append("&cart_code=").append(cartCode);

    QByteArray a ;
    if (PostMethod(url, headers, data, 1000, a))
    {
        if (a.length() > 0) // 已扫码，登录成功
        {
            QJsonObject obj = QJsonDocument::fromJson(a).object();
            if (!obj.isEmpty() && obj.contains("code"))
            {
                if (obj["code"].toInt() == 0)
                {
                    if (obj.contains("data"))
                    {
                        QJsonObject o2 = obj["data"].toObject();
                        if (o2.contains("to_url"))
                        {
                            ;
                        }
                    }
                }
            }
            qDebug("sumit order done. result %s", a.data());
            return true;
        }
    }

    return false;
}

bool MainWindow::PayPage()
{
    // 跳转支付界面
    // GET /order/topay.php?orderId=416336063806541638
    return false;
}

void MainWindow::SetCookie(QNetworkRequest &req)
{
    QList<QNetworkCookie> cookielist = qvariant_cast<QList<QNetworkCookie> >(req.header(QNetworkRequest::CookieHeader));
    for (auto c : cookielist)
    {
        qDebug("request cookie %s : %s", c.name().data(), c.value().data());
    }
    return;
    QStringList urls{"https://shop.10086.cn", "https://login.10086.cn", "https://pay.shop.10086.cn"};
    QList<QNetworkCookie> cookies;
    for (auto url : urls)
    {
        cookies.append(cookieJar->cookiesForUrl(url));
    }
    if (cookies.length() == 0)
        return;
    QVariant v;
    v.setValue(cookies);
    req.setHeader(QNetworkRequest::CookieHeader, v);

    for (auto i : cookies)
    {
        qDebug("set cookie %s : %s", i.name().data(), i.value().data());
    }
}

void MainWindow::UpdateCookie(const QNetworkReply &rep)
{
//    return;
//    QList<QByteArray> hs = rep.rawHeaderList();
    QVariant variantCookies = rep.header(QNetworkRequest::SetCookieHeader);
    QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(variantCookies);
    for (auto c : cookies)
    {
        qDebug("UpdateCookie cookie : %s", c.toRawForm().data());
//        cookieJar->insertCookie(c);
    }

//    for (auto i : cookieJar->cookiesForUrl(rep.url()))
//    {
//        qDebug("UpdateCookie cookie %s : %s", i.name().data(), i.value().data());
//    }
}

QByteArray MainWindow::GetCookieValue(const QString &url, const QByteArray &k)
{
    QByteArray ret;

    for (auto i : cookieJar->cookiesForUrl(url))
    {
        if (i.name().compare(k, Qt::CaseInsensitive) == 0)
        {
            ret = i.value();
            break;
        }
    }
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

    t.start();
    QNetworkReply *rep = manager->get(req);
    connect(rep, &QNetworkReply::finished,
            &loop, &QEventLoop::quit);
    loop.exec();

    SetCookie(req);
    if (t.isActive())
    {
        qDebug("Get error info : %s", rep->errorString().toUtf8().data());
        UpdateCookie(*rep);
        r = rep->readAll();
        rep->deleteLater();
        return true;
    }
    qDebug("Get timeout");
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

    t.start();
    QNetworkReply *rep = manager->post(req, body);
    connect(rep, &QNetworkReply::finished,
            &loop, &QEventLoop::quit);
    loop.exec();

    SetCookie(req);
    if (t.isActive())
    {
        qDebug("Post error info : %s", rep->errorString().toUtf8().data());
        UpdateCookie(*rep);
        r = rep->readAll();
        rep->deleteLater();
        return true;
    }
    qDebug("Post timeout");
    rep->deleteLater();
    return false;
}

void MainWindow::Sleep(int time)
{
    if (time <= 0)
    {
        time = 3000;
    }
    QEventLoop loop;
    QTimer timer;
    timer.setInterval(time);
    connect(&timer, &QTimer::timeout,
            &loop, &QEventLoop::quit);

    timer.start();

    loop.exec();
}

QByteArray MainWindow::GetCurrentTimeStr()
{
    return QByteArray::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
}
