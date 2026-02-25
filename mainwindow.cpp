#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include "pakemonbox.h"
#include "picturepokemon.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QAuthenticator>
#include <QByteArray>
#include <QLineEdit>
#include <QMessageBox>
//=====================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), m_nasm(new QNetworkAccessManager(this))
{
    ui->setupUi(this);

    setWindowTitle("Покемоны");
    auto allPicturePokemon = ui->pakemonBox->findChildren<PicturePokemon*>();
    for(PicturePokemon* pokemon : allPicturePokemon) {
        connect(pokemon, &PicturePokemon::clickedChanged,
                this, &MainWindow::onClickedToPicturePokemon);
    }
    connect(ui->pbn_cancellInform,&QPushButton::clicked,[this](){
        ui->pbn_next->show();
        ui->pbn_previous->show();
        ui->pbn_start->show();
    });
    connect(this,&MainWindow::completePixmapContainer,this,&MainWindow::onCompletePixmapConteiner);
    connect(ui->pbn_cancellInform,&QPushButton::clicked,[this](){ui->stackedWidget->setCurrentIndex(0);});

    QNetworkReply* reply = m_nasm->get(QNetworkRequest(QUrl("https://pokeapi.co/api/v2/pokemon/")));
    QObject::connect(reply,  &QNetworkReply::finished,[&,reply](){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray(reply->readAll()));
        QJsonObject jsonObj = jsonDoc.object();
        _nextPath=jsonObj.value("next").toString();
        _previousPath=jsonObj.value("previous").toString();
        QJsonArray _array=jsonObj.value("results").toArray();
        showPokemon(_array);
    });

    QObject::connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error)
                     , [reply](QNetworkReply::NetworkError err){
        qDebug()<<"Error:"<<reply->errorString();

    });
    QObject::connect(reply, &QNetworkReply::finished, reply, &QObject::deleteLater);
    QObject::connect(m_nasm, &QNetworkAccessManager::authenticationRequired
                     ,[reply](QNetworkReply* r, QAuthenticator* auth){
        if(reply != r)
            return;
        auth->setUser("User");
        auth->setPassword("Password");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showPokemon(const QJsonArray &_array)
{
    ui->pakemonBox->showPokemon(_array);
    auto pathPokemon=ui->pakemonBox->getPicturePaths();
    for(int i=0;i<pathPokemon.size();i++)
    {
        loadImagePokemon(pathPokemon.at(i));
    }
}

void MainWindow::loadImagePokemon(const QString &path)
{
    QNetworkReply* reply = m_nasm->get(QNetworkRequest(QUrl(path)));
    QObject::connect(reply,  &QNetworkReply::finished,[&,reply](){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray(reply->readAll()));
        QJsonObject jsonObj = jsonDoc.object();
        QJsonValue  _image=jsonObj.value("sprites");
        if(_image.isObject())
        {
            QJsonObject pokemonObj = _image.toObject();
            QString imagePokemon=pokemonObj.value("front_default").toString();
            addLoadPixMap(imagePokemon);
        }
    });

    QObject::connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error)
                     , [reply](QNetworkReply::NetworkError err){
        qDebug()<<"Error:"<<reply->errorString();
    });

    QObject::connect(reply, &QNetworkReply::finished, reply, &QObject::deleteLater);
}

void MainWindow::getInformPokemon(const QUrl &url)
{
    QNetworkReply* reply = m_nasm->get(QNetworkRequest(QUrl(url)));
    QObject::connect(reply,  &QNetworkReply::finished,[&,reply](){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray(reply->readAll()));
        QJsonObject jsonObj = jsonDoc.object();
        QString _name=jsonObj.value("name").toString();
        double _height=jsonObj.value("height").toDouble()/10.0;
        double _weight=jsonObj.value("weight").toDouble()*0.1;
        double _base_experience=jsonObj.value("base_experience").toDouble();
        double _orderInPokedox=jsonObj.value("order").toDouble();
        double _id=jsonObj.value("id").toDouble();

        ui->name_pokemon->setText(_name);
        ui->id_pokemon->setText(QString::number(_id));
        ui->height_pokemon->setText(QString::number(_height));
        ui->weight_pokemon->setText(QString::number(_weight));
        ui->order_pokedox->setText(QString::number(_orderInPokedox));
        ui->base_experience->setText(QString::number(_base_experience));
        ui->statusbar->clearMessage();
        ui->pbn_next->hide();
        ui->pbn_previous->hide();
        ui->pbn_start->hide();
        ui->stackedWidget->setCurrentIndex(1);
    });

    QObject::connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error)
                     , [reply](QNetworkReply::NetworkError err){
        qDebug()<<"Error:"<<reply->errorString();
    });
    QObject::connect(reply, &QNetworkReply::finished, reply, &QObject::deleteLater);
}

void MainWindow::onClickedToPicturePokemon()
{
    auto picturePokemon=ui->pakemonBox->findChildren<PicturePokemon*>();
    auto linkPokemon=ui->pakemonBox->findChildren<QLineEdit*>();
    QString enterLinkPokemon;
    for(int i=0; i<picturePokemon.size();i++)
    {
        if(picturePokemon[i]->property("clicked").toBool())
        {
            enterLinkPokemon=linkPokemon[i]->text();
        }
        picturePokemon[i]->setProperty("clicked",QVariant(false));
    }
    getInformPokemon(enterLinkPokemon);
}

void MainWindow::onCompletePixmapConteiner()
{
    ui->pakemonBox->setImagePokemon(pix_map);
    pix_map.clear();
    ui->statusbar->showMessage("Картинки успешно загружены");
}

void MainWindow::on_pbn_next_clicked()
{
    pix_map.clear();
    ui->statusbar->clearMessage();
    QNetworkReply* reply = m_nasm->get(QNetworkRequest(QUrl(_nextPath)));
    QObject::connect(reply,  &QNetworkReply::finished,[&,reply](){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray(reply->readAll()));
        QJsonObject jsonObj = jsonDoc.object();
        _nextPath=jsonObj.value("next").toString();
        _previousPath=jsonObj.value("previous").toString();
        QJsonArray _array=jsonObj.value("results").toArray();
        showPokemon(_array);
    });
    QObject::connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error)
                     , [reply](QNetworkReply::NetworkError err){
        qDebug()<<"Error:"<<reply->errorString();

    });
    QObject::connect(reply, &QNetworkReply::finished, reply, &QObject::deleteLater);

    ui->statusbar->showMessage("Дождитесь обновления картинок");
}

void MainWindow::on_pbn_previous_clicked()
{
    pix_map.clear();
    ui->statusbar->clearMessage();
    if(_previousPath.isEmpty())
    {
        return;
    }
    QNetworkReply* reply = m_nasm->get(QNetworkRequest(QUrl(_previousPath)));
    QObject::connect(reply,  &QNetworkReply::finished,[&,reply](){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray(reply->readAll()));
        QJsonObject jsonObj = jsonDoc.object();
        _nextPath=jsonObj.value("next").toString();
        _previousPath=jsonObj.value("previous").toString();
        QJsonArray _array=jsonObj.value("results").toArray();
        showPokemon(_array);
    });
    QObject::connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error)
                     , [reply](QNetworkReply::NetworkError err){
        qDebug()<<"Error:"<<reply->errorString();

    });
    QObject::connect(reply, &QNetworkReply::finished, reply, &QObject::deleteLater);

    ui->statusbar->showMessage("Дождитесь обновления картинок");
}

void MainWindow::addLoadPixMap(const QString &path)
{
    if (pix_map.contains(path)) {
        return;
    }
    QNetworkReply *reply = m_nasm->get(QNetworkRequest(QUrl(path)));
    connect(reply, &QNetworkReply::finished, [this, reply, path]() mutable {
        if (reply->error() != QNetworkReply::NoError) {
            pix_map.insert(path, QPixmap(":/images/default.png"));
            reply->deleteLater();
            return;
        }

        QPixmap pix;
        pix.loadFromData(reply->readAll());
        if (!pix.isNull()) {
            pix_map.insert(path, pix);
            if(pix_map.size()==20)
            {
                emit completePixmapContainer();
            }
        } else {
            pix_map.insert(path, QPixmap(":/images/default.png"));
        }

        reply->deleteLater();
    });
}

void MainWindow::on_pbn_start_clicked()
{
    pix_map.clear();
    ui->statusbar->clearMessage();

    QNetworkReply* reply = m_nasm->get(QNetworkRequest(QUrl("https://pokeapi.co/api/v2/pokemon/")));
    QObject::connect(reply,  &QNetworkReply::finished,[&,reply](){
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QByteArray(reply->readAll()));
        QJsonObject jsonObj = jsonDoc.object();
        _nextPath=jsonObj.value("next").toString();
        _previousPath=jsonObj.value("previous").toString();
        QJsonArray _array=jsonObj.value("results").toArray();
        showPokemon(_array);
    });
    QObject::connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error)
                     , [reply](QNetworkReply::NetworkError err){
        qDebug()<<"Error:"<<reply->errorString();

    });
    QObject::connect(reply, &QNetworkReply::finished, reply, &QObject::deleteLater);

    ui->statusbar->showMessage("Дождитесь обновления картинок");
}
