#include "pakemonbox.h"
#include "ui_pakemonbox.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include "pakemonbox.h"
#include <QLabel>
#include <QLineEdit>
#include <QPixmap>
#include "picturepokemon.h"
#include "pakemonbox.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonDocument>
#include <QAuthenticator>
#include <QByteArray>
//=====================================
PakemonBox::PakemonBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PakemonBox)
{
    ui->setupUi(this);
}

PakemonBox::~PakemonBox()
{
    delete ui;
}

void PakemonBox::showPokemon(const QJsonArray &_array)
{
    picturePaths.clear();
    auto pokemons=this->findChildren<QLabel*>();
    auto pokemons_link=this->findChildren<QLineEdit*>();
    QList<QLabel*> normalLabels;
    for(QLabel* label : pokemons) {
        if (!label->property("isPicturePokemon").toBool()) {
            normalLabels.append(label);
        }
    }

    for(const QJsonValue &item:_array)
    {
        if (item.isObject()) {
            QJsonObject pokemonObj = item.toObject();
            QString name = pokemonObj.value("name").toString();
            QString url = pokemonObj.value("url").toString();
            picturePaths.append(url);
            normalLabels.first()->setText(name);
            pokemons_link.first()->setText(url);
            normalLabels.pop_front();
            pokemons_link.pop_front();
        }
    }
}

void PakemonBox::setImagePokemon(const QMap<QString,QPixmap> &_allPix)
{
    auto _pix=_allPix;
    auto pokemons = this->findChildren<PicturePokemon*>();
    auto link_pokemons = this->findChildren<QLineEdit*>();
    for(int i = 0; i < pokemons.size(); i++) {
        PicturePokemon* pokemonLabel = pokemons[i];
        QLineEdit* linkEdit = link_pokemons[i];

        QString linkText = linkEdit->text();
        QString searchKey = linkText.right(11);
        for(auto it = _pix.begin(); it != _pix.end(); ++it) {
            QString pixKey = it.key().left(it.key().size() - 4);
            pixKey=pixKey.right(9);
            if( linkText.contains(pixKey, Qt::CaseInsensitive)) {
                pokemonLabel->setPixmap(it.value().scaled(100, 100, Qt::KeepAspectRatio));
                _pix.erase(it);
                break;
            }
        }
    }
}

QVector<QString> PakemonBox::getPicturePaths() const
{
    return picturePaths;
}
