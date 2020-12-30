#include "projection.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QStringListModel>
#include <QTableView>
#include <QTableWidget>
#include <QFileDialog>
#include <QCoreApplication>
#include <QTextStream>

projection::projection()
{

}

projection::projection(int num_projection, int id, QDateTime date_projection, int num_salle, int capacite_salle)
{
    this->num_projection = num_projection;
    this->id = id;
    this->date_projection = date_projection;
    this->num_salle = num_salle;
    this->capacite_salle = capacite_salle;

}

bool projection::ajouter_p()
{
    QSqlQuery query;
    query.prepare("INSERT into PROJECTION (num_projection, id, date_projection, num_salle, capacite_salle)"
              "Values(:num_projection, :id, :date_projection, :num_salle, :capacite_salle)");
    query.bindValue(":num_projection", num_projection);
    query.bindValue(":id", id);
    query.bindValue(":date_projection", date_projection);
    query.bindValue(":num_salle", num_salle);
    query.bindValue(":capacite_salle", capacite_salle);

    return query.exec();
}

QSqlQueryModel * projection::afficher_p()
{
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery(" SELECT num_projection, nom, date_projection, num_salle, capacite_salle from projection inner join film"
                    " on projection.id = film.id");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Num_projection"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom film"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Date_projection"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Num_salle"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Capacite_salle"));
    return model;
}

bool projection::supprimer_p(int num_projection)
{
    QSqlQuery query;
    QString res = QString::number(num_projection);
    query.prepare("DELETE from PROJECTION where num_projection=:num_projection");
    query.bindValue(":num_projection", res);
    return query.exec();
}

QSqlQueryModel * projection::rechercher_p(int num_projection, int num_salle, QDateTime date_projection)
{
    QSqlQuery *query=new QSqlQuery();
    query->prepare("SELECT * from PROJECTION where num_projection=:num_projection and num_salle=:num_salle and date_projection=:date_projection");
    query->bindValue(":num_projection",num_projection);
    query->bindValue(":num_salle",num_salle);
    query->bindValue(":date_projection",date_projection);
    query->exec();
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(*query);
    return model;
}

QSqlQueryModel * projection::trier_p(QString crit)
{
    QSqlQuery *query=new QSqlQuery();
    QSqlQueryModel *model=new QSqlQueryModel();
    query->prepare("select * from projection order by "+crit+" ");
    query->exec();
    model->setQuery(*query);
    return model;
}

void projection::exporterExcel_p(QTableView *table)
{
    QString filters("CSV files (*.csv);;All files (*.*)");
    QString defaultFilter("CSV files (*.csv)");
    QString fileName = QFileDialog::getSaveFileName(0, "Save file", QCoreApplication::applicationDirPath(),
                                                        filters, &defaultFilter);
    QFile file(fileName);
    QAbstractItemModel *model =  table->model();
    if (file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream data(&file);
        QStringList strList;
        for (int i = 0; i < model->columnCount(); i++)
        {
            if (model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString().length() > 0)
                strList.append("\"" + model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString() + "\"");
            else
                strList.append("");
         }
         data << strList.join(";") << "\n";
         for (int i = 0; i < model->rowCount(); i++)
         {
            strList.clear();
            for (int j = 0; j < model->columnCount(); j++)
            {

                if (model->data(model->index(i, j)).toString().length() > 0)
                    strList.append("\"" + model->data(model->index(i, j)).toString() + "\"");
                else
                    strList.append("");
             }
                data << strList.join(";") + "\n";
            }
            file.close();
        }
}

bool projection::num_existe(int num_projection)
{
    QSqlQuery query;
    int n=0;
    query.prepare("select * from projection where num_projection=:num_projection");
    query.bindValue(":num_projection",num_projection);
    query.exec();
    while(query.next()){
        n++;
    }
    if (n)
    {
        return true;
    }
    return false;
}
