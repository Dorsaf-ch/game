#ifndef PROJECTION_H
#define PROJECTION_H
#include <QDateTimeEdit>
#include <QSqlQueryModel>
#include <QTableView>

class projection
{
private:
    int num_projection;
    int id;
    QDateTime date_projection;
    int num_salle;
    int capacite_salle;

public:
    projection();
    projection(int, int, QDateTime, int, int);
    // SETTERS
    void set_num_projection(int n){num_projection = n;}
    void set_id(int i){id = i;}
    void set_date_projection(QDateTime d){date_projection = d;}
    void set_num_salle(int s){num_salle = s;}
    void set_capacite_salle(int c){capacite_salle = c;}

    //GETTERS
    int get_num_projection(){return num_projection;}
    int get_id(){return id;}
    QDateTime get_date_projection(){return date_projection;}
    int get_num_salle(){return num_salle;}
    int get_capacite_salle(){return capacite_salle;}


    bool ajouter_p();
    QSqlQueryModel * afficher_p();
    bool supprimer_p(int);
    QSqlQueryModel * rechercher_p(int, int, QDateTime);
    QSqlQueryModel * trier_p(QString);
    void exporterExcel_p(QTableView *table);
    bool num_existe(int);


};

#endif // PROJECTION_H
