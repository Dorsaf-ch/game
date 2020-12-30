#include "stats.h"
#include "ui_stats.h"
#include <QSqlQuery>
#include "film.h"

stats::stats(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stats)
{
    ui->setupUi(this);
}

stats::~stats()
{
    delete ui;
}

void stats::make()
{
        int horreur;
        int comedie;
        int drama;
        int action;
        int aventure;
        int fantasie;
        int disney;
        int total;
        int historique;
        QString Horreur;
        QString Comedie;
        QString Drama;
        QString Action;
        QString Aventure;
        QString Fantasie;
        QString Disney;
        QString Historique;
        QSqlQuery qry;

        qry.prepare("SELECT COUNT(id) FROM film where genre ='Horreur' ");
        qry.exec();
        qry.first() ;
        horreur=(qry.value(0).toInt());

        qry.prepare("SELECT COUNT(id) FROM film where genre ='Comedie' ");
        qry.exec();
        qry.first() ;
        comedie=(qry.value(0).toInt());

        qry.prepare("SELECT COUNT(id) FROM film where genre ='Drama' ");
        qry.exec();
        qry.first() ;
        drama=(qry.value(0).toInt());

        qry.prepare("SELECT COUNT(id) FROM film where genre ='Action' ");
        qry.exec();
        qry.first() ;
        action=(qry.value(0).toInt());

        qry.prepare("SELECT COUNT(id) FROM film where genre ='Fantasie' ");
        qry.exec();
        qry.first() ;
        fantasie=(qry.value(0).toInt());

        qry.prepare("SELECT COUNT(id) FROM film where genre ='Disney' ");
        qry.exec();
        qry.first() ;
        disney=(qry.value(0).toInt());

        qry.prepare("SELECT COUNT(id) FROM film where genre ='Aventure' ");
        qry.exec();
        qry.first() ;
        aventure=(qry.value(0).toInt());

        qry.prepare("SELECT COUNT(id) FROM film where genre ='Historique' ");
        qry.exec();
        qry.first() ;
        historique=(qry.value(0).toInt());


        qry.prepare("SELECT COUNT(id) FROM film  ");
        qry.exec();
        qry.first() ;
        total=(qry.value(0).toInt());

        horreur=((double)horreur/(double)total)*100;
        comedie=((double)comedie/(double)total)*100;
        drama=((double)drama/(double)total)*100;
        aventure=((double)aventure/(double)total)*100;
        fantasie=((double)fantasie/(double)total)*100;
        action=((double)action/(double)total)*100;
        disney=((double)disney/(double)total)*100;
        historique=((double)historique/(double)total)*100;

        Horreur= QString::number(horreur);
        Comedie=QString::number(comedie);
        Drama=QString::number(drama);
        Aventure=QString::number(aventure);
        Fantasie=QString::number(fantasie);
        Action=QString::number(action);
        Disney=QString::number(disney);
        Historique=QString::number(historique);

        QPieSeries *series;
         series= new  QPieSeries();
         series->append("HORREUR"+Horreur+"%",horreur);
         series->append("COMEDIE"+Comedie+"%",comedie);
         series->append("DRAMA"+Drama+"%",drama);
         series->append("FANTASIE"+Fantasie+"%",fantasie);
         series->append("ACTION"+Action+"%",action);
         series->append("AVENTURE"+Disney+"%",disney);
         series->append("DISNEY"+Aventure+"%",aventure);
         series->append("HISTORIQUE"+Historique+"%",historique);

         if (horreur!=0)
         {
         QPieSlice *slice0 = series->slices().at(0);
         slice0->setExploded();
         slice0->setLabelVisible();
         slice0->setPen(QPen(Qt::darkGray, 2));
         slice0->setBrush(Qt::gray);
         }
         if (comedie!=0)
        {
          QPieSlice *slice1 = series->slices().at(1);
          slice1->setExploded();
          slice1->setLabelVisible();
          slice1->setPen(QPen(Qt::darkRed, 2));
          slice1->setBrush(Qt::red);
        }
         if (drama!=0)
         {
          QPieSlice *slice2 = series->slices().at(2);
          slice2->setExploded();
          slice2->setLabelVisible();
          slice2->setPen(QPen(Qt::darkGreen, 2));
          slice2->setBrush(Qt::green);
        }
         if (fantasie!=0)
        {
          QPieSlice *slice3 = series->slices().at(3);
          slice3->setExploded();
          slice3->setLabelVisible();
          slice3->setPen(QPen(Qt::darkBlue, 2));
          slice3->setBrush(Qt::blue);
         }
          if (action!=0)
        {
          QPieSlice *slice4 = series->slices().at(4);
          slice4->setExploded();
          slice4->setLabelVisible();
          slice4->setPen(QPen(Qt::darkCyan, 2));
          slice4->setBrush(Qt::cyan);
        }
          if (aventure!=0)
          {
          QPieSlice *slice5 = series->slices().at(5);
          slice5->setExploded();
          slice5->setLabelVisible();
          slice5->setPen(QPen(Qt::darkYellow, 2));
          slice5->setBrush(Qt::yellow);
        }
          if (disney!=0)
           {
          QPieSlice *slice6 = series->slices().at(6);
          slice6->setExploded();
          slice6->setLabelVisible();
          slice6->setPen(QPen(Qt::darkMagenta, 2));
          slice6->setBrush(Qt::magenta);
        }
        if (historique!=0)
        {
          QPieSlice *slice7 = series->slices().at(7);
          slice7->setExploded();
          slice7->setLabelVisible();
          slice7->setPen(QPen(Qt::darkBlue, 2));
          slice7->setBrush(Qt::black);
        }
          QChart *chart = new QChart();
          chart->addSeries(series);
          chart->setTitle("Statistiques sur les genres les plus mawjoudin ");
          chart->legend()->show();
          QChartView *chartView = new QChartView(chart);
          chartView->setRenderHint(QPainter::Antialiasing);
          ui->verticalLayout->addWidget(chartView);

}
