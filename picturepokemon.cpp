#include "picturepokemon.h"
#include "ui_picturepokemon.h"
#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
//=====================================
PicturePokemon::PicturePokemon(QWidget *parent) :
    QLabel(parent),
    ui(new Ui::PicturePokemon)
{
    ui->setupUi(this);
    setProperty("isPicturePokemon", true);
    setMouseTracking(true);
    setClicked(false);
}

PicturePokemon::~PicturePokemon()
{
    delete ui;
}

bool PicturePokemon::isClicked() const
{
    return m_clicked;
}

void PicturePokemon::setClicked(bool clicked)
{
    if(m_clicked != clicked) {
        m_clicked = clicked;
        emit clickedChanged();
    }
}
void PicturePokemon::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        setClicked(true);
    }
    QLabel::mousePressEvent(event);
}
