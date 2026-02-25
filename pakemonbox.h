#ifndef PAKEMONBOX_H
#define PAKEMONBOX_H

#include <QWidget>
#include <QNetworkAccessManager>
namespace Ui {
class PakemonBox;
}

class PakemonBox : public QWidget
{
    Q_OBJECT

public:
    explicit PakemonBox(QWidget *parent = nullptr);
    ~PakemonBox();
    void showPokemon(const QJsonArray& _array);
    void setImagePokemon(const QMap<QString,QPixmap> &_allPix);
    QVector<QString> getPicturePaths()const;

private:
    QVector<QString> picturePaths;
    Ui::PakemonBox *ui;
};

#endif // PAKEMONBOX_H
