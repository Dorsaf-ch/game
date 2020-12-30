#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "film.h"
#include "projection.h"
#include "arduino.h"
#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QVariant Data(const QModelIndex &index, int role) const;



;

private slots:
    void update_label();

    void on_ajouter_clicked();

    void on_tableView_films_activated(const QModelIndex &index);

    void on_pushButton_suppFilm_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_clicked();

    void on_pushButton_pdfFilms_clicked();

    void on_pushButton_tri_clicked();

    void on_pushButton_ajouter_proj_clicked();

    void on_tableView_projections_activated(const QModelIndex &index);

    void on_pushButton_update_proj_clicked();

    void on_pushButton_supp_proj_clicked();

    void on_pushButton_2_clicked();

    void on_rech_proj_clicked();

    void on_tri_proj_clicked();

    void on_exporterPdf_proj_clicked();

    void on_exporterExcel_proj_clicked();

    void on_pushButton_afficherF_clicked();

    void on_pushButton_afficherP_clicked();

    void on_night_mode_clicked();

    void on_daymode_clicked();

    void on_statsFilms_clicked();

    void on_login_clicked();

    void on_menuPROG2_clicked();

    void on_returnPROG_clicked();

    void on_logout_clicked();

    void on_nightmode_clicked();

    void on_pushButton_3_clicked();

    void on_menuNOTIF1_clicked();

    void on_menuNOTIF2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_logout_clicked();

    void on_menuPROG1_clicked();

    void reset();
private:
    Ui::MainWindow *ui;
    film tmpFilm;
    projection tmpProjection;
    //arduino A;
    QByteArray data;
    QRegExp duree_reg;
    QString tmp="";
    arduino A;

    //QSound *son;

};
#endif // MAINWINDOW_H
