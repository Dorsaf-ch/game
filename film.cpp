#include "film.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QStringListModel>
#include <QSqlRecord>
#include <QFile>
#include <QFileDialog>
#include <QTableView>
#include <QTextStream>
#include <QCoreApplication>
#include <QtPrintSupport/QPrinter>
#include <QTextDocument>
#include <QTableWidget>
#include <QtPrintSupport/QPrintDialog>


film::film()
{

}

film::film(int id, QString nom, QString genre, QString description, QString duree, QDate date_sortie)
{
    this->id = id;
    this->nom = nom;
    this->genre = genre;
    this->description = description;
    this->duree = duree;
    this->date_sortie = date_sortie;
}

bool film::ajouter()
{
    QSqlQuery query;
    query.prepare("Insert into film (id, nom, genre,description, duree, date_sortie)"
                  "Values(:id, :nom, :genre, :description, :duree, :date_sortie)");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":genre", genre);
    query.bindValue(":description", description);
    query.bindValue(":duree", duree);
    query.bindValue(":date_sortie", date_sortie);
    return query.exec();

}

QSqlQueryModel * film::afficher()
{
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("select * from film");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Genre"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Duree"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date_sortie"));

    return model;
}

bool film::supprimer(int id)
{
    QSqlQuery query;
    QString res = QString::number(id);
    query.prepare("DELETE from film where id = :id");
    query.bindValue(":id", res);
    return query.exec();
}

QSqlQueryModel * film::rechercher_multi(QString genre,QString nom,QDate date_sortie)
{
    QSqlQuery *query=new QSqlQuery();
    query->prepare("select * from film where nom=:nom and genre=:genre and date_sortie=:date_sortie");
    query->bindValue(":nom",nom);
    query->bindValue(":genre",genre);
    query->bindValue(":date_sortie",date_sortie);
    query->exec();
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(*query);
    return model;
}

QSqlQueryModel * film::trier(QString crit)
{
    QSqlQuery *query=new QSqlQuery();
       QSqlQueryModel *model=new QSqlQueryModel();
       query->prepare("select * from film order by "+crit+"");
       query->exec();
       model->setQuery(*query);
       return model;
}

void film::exporter_excel(QTableView *table)
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

bool film::id_existe(int id)
{
    QSqlQuery query;
    int n=0;
    query.prepare("select * from film where id=:id");
    query.bindValue(":id",id);
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
