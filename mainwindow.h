#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QNetworkAccessManager>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showPokemon(const QJsonArray& _array);
    void loadImagePokemon(const QString& path);
    void getInformPokemon(const QUrl& url);

signals:
    void completePixmapContainer();

private slots:
    void onClickedToPicturePokemon();
    void onCompletePixmapConteiner();
    void addLoadPixMap(const QString& path);
    void on_pbn_next_clicked();
    void on_pbn_previous_clicked();
    void on_pbn_start_clicked();

private:
    QString _nextPath;
    QString _previousPath;
    QVector<QString> _imagePaths;
    QMap<QString,QPixmap> pix_map;
    QNetworkAccessManager *m_nasm;

    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
