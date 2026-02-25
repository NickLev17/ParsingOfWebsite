#ifndef PICTUREPOKEMON_H
#define PICTUREPOKEMON_H

#include <QWidget>
#include <QPixmap>
#include <QLabel>
namespace Ui {
class PicturePokemon;
}

class PicturePokemon : public QLabel
{
    Q_OBJECT

public:
    Q_PROPERTY(bool clicked READ isClicked WRITE setClicked NOTIFY clickedChanged);
    explicit PicturePokemon(QWidget *parent = nullptr);
    ~PicturePokemon();
    bool isClicked() const;
    void setClicked(bool clicked);

signals:
    void clickedChanged();
private:
    Ui::PicturePokemon *ui;
    bool m_clicked;
protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // PICTUREPOKEMON_H
