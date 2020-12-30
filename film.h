#ifndef FILM_H
#define FILM_H
#include <QString>
#include <QDate>
#include <QSqlQueryModel>
#include <QStringListModel>
#include <QTableView>
#include <QtPrintSupport/QPrinter>
#include <QSqlTableModel>
class film
{
private:
    int id;
    QString nom, genre, description;
    QString duree;
    QDate date_sortie;
    QStringList stringList;
public:
    film();
    film(int, QString, QString, QString, QString, QDate);
    void set_id(int i){id = i;}
    void set_nom(QString n){nom = n;}
    void set_genre(QString g){genre = g;}
    void set_description(QString d){description = d;}
    void set_duree(QString d){duree = d;}
    void set_date_sortie(QDate ds){date_sortie = ds;}

    int get_id(){return id;}
    QString get_nom(){return nom;}
    QString get_genre(){return genre;}
    QString get_description(){return description;}
    QString get_duree(){return duree;}
    QDate get_date_sortie(){return date_sortie;}

    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    QSqlQueryModel * rechercher_multi(QString, QString, QDate);
    QSqlQueryModel * trier(QString);
    void exporter_excel(QTableView *table);
    bool id_existe(int);
};

#endif // FILM_H
