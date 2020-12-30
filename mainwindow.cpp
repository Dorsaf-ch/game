#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "film.h"
#include "projection.h"
#include "stats.h"
#include "arduino.h"
#include <QMessageBox>
#include <QDate>
#include <QString>
#include <QDateEdit>
#include <QModelIndex>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QPropertyAnimation>
#include <QPrintDialog>
#include <QTextStream>
#include <QRegExp>
#include <QMediaPlayer>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    duree_reg=QRegExp("[0-4]+h[0-5]+[0-9]+m$");
    int const n=0;

    ui->setupUi(this);

    //this->setStyleSheet("background-color: rgb(129, 173, 186); color: rgb(0, 0, 0);");

    //MUSIC
    QMediaPlayer *player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("C:/Users/Asus/Documents/cine/Beautiful-cinematic-piano-background-music.mp3"));
    player->setVolume(n);
    player->play();
    ui->spinBox->setRange(0,100);
    ui->spinBox->setValue(n);
    ui->horizontalSlider->setRange(0,100);
    ui->horizontalSlider->setValue(n);
    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->spinBox,SLOT(setValue(int)));
    QObject::connect(ui->spinBox,SIGNAL(valueChanged(int)),ui->horizontalSlider,SLOT(setValue(int)));
    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),player,SLOT(setVolume(int)));
    QObject::connect(ui->spinBox,SIGNAL(valueChanged(int)),player,SLOT(setVolume(int)));


    ui->lineEdit_id->setPlaceholderText("ID...");
    ui->lineEdit_nom->setPlaceholderText("Nom...");
    ui->textEdit_description->setPlaceholderText("Desc...");
    ui->lineEdit_duree->setPlaceholderText(".h..m");
    ui->lineEdit_nump->setPlaceholderText("Num proj...");
    ui->lineEdit_capacite->setPlaceholderText("Capacite...");
    ui->lineEdit_rechnom->setPlaceholderText("Nom film...");
    ui->lineEdit_rech_numP->setPlaceholderText("Num proj...");
    ui->lineEdit_username->setPlaceholderText("USERNAME..");
    ui->lineEdit_password->setPlaceholderText("PASSWORD..");

    //Background
    QPixmap bkgnd("C:/Users/Asus/Desktop/smart_cinema/Integration_Smart_Cinema/background.jpg");
       bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
       QPalette palette;
       palette.setBrush(QPalette::Background, bkgnd);
       this->setPalette(palette);

    //Controle de saisie max
    ui->lineEdit_id->setMaxLength(8);
    ui->lineEdit_duree->setMaxLength(5);
    ui->dateEdit_ds->setMinimumDate(QDate::currentDate().addYears(-100));
    ui->dateEdit_ds->setMaximumDate(QDate::currentDate().addDays(1));
    ui->dateEdit_ds->setDisplayFormat("dd.MM.yyyy");
    ui->dateEdit_rech->setMinimumDate(QDate::currentDate().addYears(-100));
    ui->dateEdit_rech->setMaximumDate(QDate::currentDate().addDays(1));

    ui->lineEdit_nump->setMaxLength(8);
    ui->lineEdit_capacite->setMaxLength(3);
    ui->spinBox_salle->setMaximum(40);
    ui->dateTimeEdit_datep->setMinimumDateTime(QDateTime::currentDateTime().addYears(-1));
    ui->dateTimeEdit_datep->setMaximumDateTime(QDateTime::currentDateTime().addYears(3));
    ui->dateTimeEdit_datep->setDisplayFormat("dd.MM.yyyy hh:mm");
    ui->lineEdit_rech_numP->setMaxLength(8);
    ui->spinBox_rechsalle->setMaximum(40);
    ui->dateTimeEdit_rechP->setMinimumDateTime(QDateTime::currentDateTime().addYears(-2));
    ui->dateTimeEdit_rechP->setMaximumDateTime(QDateTime::currentDateTime().addYears(3));

    //Selection des tables
    ui->tableView_films->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_films->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_projections->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_projections->setSelectionMode(QAbstractItemView::SingleSelection);

    //Affichage des tables
    ui->tableView_films->setModel(tmpFilm.afficher());
    ui->tableView_projections->setModel(tmpProjection.afficher_p());

    int ret=A.connect_arduino();
           switch (ret)
           {
           case 0:
               qDebug() << "arduino is available and connected to:" << A.getarduino_port_name();
               break;
           case 1:
               qDebug() << "arduino is available but not connected to:" << A.getarduino_port_name();
               break;
           case -1:
               qDebug() << "arduino is not available";
               break;
           }

           QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label()));

    int nb = ui->tableView_films->model()->rowCount();
    for (int i=0; i<nb; i++)
      {
          QString id = ui->tableView_films->model()->index(i, 0).data().toString();
          ui->comboBox_idF_proj->addItem(id);
      }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_login_clicked()
{
    if(ui->lineEdit_username->text()=="admin" && ui->lineEdit_password->text()== "admin")
    {
        ui->stackedWidget->setCurrentIndex(1);
        reset();
    }
    else if(ui->lineEdit_username->text()=="agent" && ui->lineEdit_password->text()== "agent")
    {
        ui->stackedWidget->setCurrentIndex(2);
        reset();
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Username ou mot de passe incorrect") ,
                     QObject::tr("Connexion failed.\n" "Click cancel to exit"), QMessageBox::Cancel);
        reset();
    }
}
void MainWindow::reset()
{
    foreach(QLineEdit *widget, this->findChildren<QLineEdit*>())
        {
        widget->clear();
        }
}
void MainWindow::on_menuPROG1_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_menuPROG2_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_logout_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_logout_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_menuNOTIF1_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_menuNOTIF2_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}


// -------------------DORSAF------------//

void MainWindow::on_ajouter_clicked()
{
    bool duree_verif = duree_reg.exactMatch(ui->lineEdit_duree->text()); 
    int id = ui->lineEdit_id->text().toInt();
    QString nom = ui->lineEdit_nom->text();
    QString genre = ui->comboBox_genre->currentText();
    QString description = ui->textEdit_description->toPlainText();
    QString duree = ui->lineEdit_duree->text();
    QDate date_sortie = ui->dateEdit_ds->date();
    film f(id, nom, genre, description, duree, date_sortie);
    bool existe = tmpFilm.id_existe(id);
    if(existe)
    {
        ui->lineEdit_id->setStyleSheet("color: red");
        ui->statusbar->showMessage("ID EXISTE!");
    }
    if (id>0 && nom!="" && description!= "" && duree!="" && duree_verif && !existe)
    {
             bool test = f.ajouter();
             if(test)
             {
                    ui->tableView_films->setModel(tmpFilm.afficher());
                    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;
                    notifyIcon->show();
                    notifyIcon->setIcon(QIcon("icone.png"));
                    notifyIcon->showMessage("GESTION FILMS ","Film Ajouté avec succès",QSystemTrayIcon::Information,15000);
                    ui->lineEdit_duree->setStyleSheet("color: black");
                    ui->lineEdit_id->setStyleSheet("color: black");
                    ui->lineEdit_nom->setStyleSheet("color: black");
                    ui->textEdit_description->setStyleSheet("color: black");
                    foreach(QLineEdit *widget, this->findChildren<QLineEdit*>())
                        {
                        widget->clear();
                        }
             }
     }
     else
     {
        if (!duree_verif)
        {
            ui->lineEdit_duree->setStyleSheet("color: red");
        }
        if(id<=0)
        {
            ui->lineEdit_id->setStyleSheet("color: red");
        }
        if(nom=="")
        {
            ui->lineEdit_nom->setStyleSheet("color: red");
        }
        if(description == "")
        {
            ui->textEdit_description->setStyleSheet("color: red");
        }
        if (!duree_verif && id <=0 && nom=="" && description=="")
        {
            ui->statusbar->showMessage("Tout les champs sont invalides.");
        }
      }
}


void MainWindow::on_tableView_films_activated(const QModelIndex &index)
{
/*
    ui->tableView_films->setStyleSheet("QTableView { selection-color: black; selection-background-color: black; }");
*/
}

void MainWindow::on_pushButton_update_clicked()
{
    QMediaPlayer * sound = new QMediaPlayer();
    sound->setMedia(QUrl("C:/Users/Asus/Documents/cine/click2.mp3"));
    sound->play();
    if (ui->pushButton_update->isEnabled())
    {
        ui->pushButton_update->setDisabled(true);
        QApplication::processEvents();
        ui->pushButton_update->setText("Modifiable");
        QSqlTableModel *tableModel= new QSqlTableModel();
        tableModel->setTable("FILM");
        tableModel->select();
        ui->tableView_films->setModel(tableModel);
    }
    ui->pushButton_update->setEnabled(true);
    ui->pushButton_update->setText("Modifier");
}


void MainWindow::on_pushButton_suppFilm_clicked()
{
    QItemSelectionModel *select = ui->tableView_films->selectionModel();

      int id =select->selectedRows(0).value(0).data().toInt();
      if(tmpFilm.supprimer(id))
      {
        ui->tableView_films->setModel(tmpFilm.afficher());
        ui->statusbar->showMessage("Film supprimé avec succès.");
    }
    else
    {
          ui->statusbar->showMessage("Erreur de supression. Film existe dans une projection.");
    }
}


void MainWindow::on_pushButton_clicked()
{
    QMediaPlayer * sound = new QMediaPlayer();
    sound->setMedia(QUrl("C:/Users/Asus/Documents/cine/click3.mp3"));
    sound->play();
    QString genre = ui->comboBox_rechF->currentText();
    QString nom=ui->lineEdit_rechnom->text();
    QDate date_sortie=ui->dateEdit_rech->date();
    QSqlQueryModel *rech=tmpFilm.rechercher_multi(genre,nom,date_sortie);
    if(rech)
    {
        ui->tableView_films->setModel(rech);
    }
    else
    {
        ui->tableView_films->setModel(tmpFilm.afficher());
    }
}

void MainWindow::on_pushButton_tri_clicked()
{
    QMediaPlayer * sound = new QMediaPlayer();
       sound->setMedia(QUrl("C:/Users/Asus/Documents/cine/click3.mp3"));
       sound->play();
    if (ui->checkBox_idTri->isChecked())
    {
        ui->tableView_films->setModel(tmpFilm.trier("id"));
    }
    else if (ui->checkBox_nomTri->isChecked())
    {
        ui->tableView_films->setModel(tmpFilm.trier("nom"));
    }
    else if (ui->checkBox_dateTri->isChecked())
    {
        ui->tableView_films->setModel(tmpFilm.trier("date_sortie"));
    }
}


void MainWindow::on_pushButton_pdfFilms_clicked()
{
       QMediaPlayer * sound = new QMediaPlayer();
       sound->setMedia(QUrl("C:/Users/Asus/Documents/cine/click3.mp3"));
       sound->play();
       QString strStream;
       QTextStream out(&strStream);
       const int rowCount = ui->tableView_films->model()->rowCount();
       const int columnCount =ui->tableView_films->model()->columnCount();
           out <<  "<html>\n"
                   "<head>\n"
                   "<meta Content=\"Text/html; charset=Windows-1251\">\n"
               <<  QString("<title>%1</title>\n").arg("film")
               <<  "</head>\n"
                   "<body bgcolor=#C5D4F6 link=#5000A0>\n"
                   "<img src='C:/Users/Asus/Documents/cine/hexa_script.png' width='100' height='100'>\n"
                   "<h1>Liste des films</h1>"
                   "<table border=1 cellspacing=0 cellpadding=2>\n";

              // headers
            out << "<thead><tr bgcolor=#f0f0f0>";
            for (int column = 0; column < columnCount; column++)
                 if (!ui->tableView_films->isColumnHidden(column))
                       out << QString("<th>%1</th>").arg(ui->tableView_films->model()->headerData(column, Qt::Horizontal).toString());
                        out << "</tr></thead>\n";
               // data table
             for (int row = 0; row < rowCount; row++) {
                  out << "<tr>";
                   for (int column = 0; column < columnCount; column++) {
                      if (!ui->tableView_films->isColumnHidden(column)) {
                           QString data = ui->tableView_films->model()->data(ui->tableView_films->model()->index(row, column)).toString().simplified();
                                       out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                       }
               }
               out << "</tr>\n";
               }
               out <<  "</table>\n"
                       "</body>\n"
                       "</html>\n";

         QTextDocument *document = new QTextDocument();
         document->setHtml(strStream);
         QPrinter printer;
         QPrintDialog *dialog = new QPrintDialog(&printer, NULL);
         if (dialog->exec() == QDialog::Accepted) {
                document->print(&printer);
         }
}


void MainWindow::on_pushButton_2_clicked()
{
    QMediaPlayer * sound = new QMediaPlayer();
    sound->setMedia(QUrl("C:/Users/Asus/Documents/cine/click3.mp3"));
    sound->play();
    tmpFilm.exporter_excel(ui->tableView_films);
}

void MainWindow::on_statsFilms_clicked()
{
    stats *w = new stats();
               w->make();
               w->show();
}


// ----------------PROJECTION-----------------//



void MainWindow::on_pushButton_ajouter_proj_clicked()
{
    int num_projection = ui->lineEdit_nump->text().toInt();
    int id = ui->comboBox_idF_proj->currentText().toInt();
    QDateTime date_projection = ui->dateTimeEdit_datep->dateTime();
    int num_salle = ui->spinBox_salle->text().toInt();
    int capacite_salle = ui->lineEdit_capacite->text().toInt();
    projection p(num_projection, id, date_projection, num_salle, capacite_salle);
    bool existe = tmpProjection.num_existe(num_projection);
    if(existe)
    {
        ui->lineEdit_nump->setStyleSheet("color: red");
        ui->statusbar->showMessage("NUM PROJECTION EXISTE!");
    }
    if(num_projection>0 && num_salle !=0 && capacite_salle<500 && capacite_salle !=0 && !existe)
    {
        bool test = p.ajouter_p();
        if(test)
        {
            ui->tableView_projections->setModel(tmpProjection.afficher_p());
            ui->lineEdit_nump->setStyleSheet("color: black");
            ui->lineEdit_capacite->setStyleSheet("color: black");
            QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;
            notifyIcon->show();
            notifyIcon->setIcon(QIcon("icone.png"));
            notifyIcon->showMessage("GESTION PROJECTIONS ","Projection Ajoutée avec succès",QSystemTrayIcon::Information,15000);                foreach(QLineEdit *widget, this->findChildren<QLineEdit*>())
                {
                 widget->clear();
                }
        }
    }
    else
    {
        if(num_projection <= 0)
        {
            ui->lineEdit_nump->setStyleSheet("color: red");
            ui->statusbar->showMessage("Numero projection invalide.");
        }
        if(capacite_salle <= 0)
        {
            ui->lineEdit_capacite->setStyleSheet("color: red");
            ui->statusbar->showMessage("Capacité salle invalide.");
        }
        if(num_salle == 0)
        {
            ui->spinBox_salle->setStyleSheet("color: red");
            ui->statusbar->showMessage("Numero salle vide.");
        }
        if(capacite_salle <=0 && num_projection<=0 && num_salle == 0)
        {
            ui->statusbar->showMessage("Les champs sont invalides.");
        }
    }
}



void MainWindow::on_tableView_projections_activated(const QModelIndex &index)
{
}

void MainWindow::on_pushButton_update_proj_clicked()
{
    QMediaPlayer * sound = new QMediaPlayer();
       sound->setMedia(QUrl("C:/Users/Asus/Documents/cine/click2.mp3"));
       sound->play();
    if (ui->pushButton_update->isEnabled())
    {
        ui->pushButton_update_proj->setDisabled(true);
        QApplication::processEvents();
        ui->pushButton_update_proj->setText("Modifiable");
        ui->statusbar->showMessage("Veuillez selectionner les champs a changer.");
        QSqlTableModel *tableModel= new QSqlTableModel();
        tableModel->setTable("PROJECTION");
        tableModel->select();
        ui->tableView_projections->setModel(tableModel);
    }
    ui->pushButton_update_proj->setEnabled(true);
    ui->pushButton_update_proj->setText("Modifier");
}


void MainWindow::on_pushButton_supp_proj_clicked()
{
    QItemSelectionModel *select = ui->tableView_projections->selectionModel();
      int num_projection =select->selectedRows(0).value(0).data().toInt();

      if(tmpProjection.supprimer_p(num_projection))
      {
          ui->tableView_projections->setModel(tmpProjection.afficher_p());
          ui->statusbar->showMessage("Projection supprimée avec succès.");
      }
      else
      {
          ui->statusbar->showMessage("Erreur de suppression.");
      }
}

void MainWindow::on_rech_proj_clicked()
{
    QMediaPlayer * sound = new QMediaPlayer();
       sound->setMedia(QUrl("C:/Users/Asus/Documents/cine/click3.mp3"));
       sound->play();
    int num_projection = ui->lineEdit_rech_numP->text().toInt();
    int num_salle = ui->spinBox_rechsalle->text().toInt();
    QDateTime date_projection = ui->dateTimeEdit_rechP->dateTime();
    QSqlQueryModel *rech=tmpProjection.rechercher_p(num_projection ,num_salle, date_projection);
    if(rech)
    {
        ui->tableView_projections->setModel(rech);
    }
    else
    {
        ui->tableView_projections->setModel(tmpProjection.afficher_p());
    }
}

void MainWindow::on_tri_proj_clicked()
{
    QMediaPlayer * sound = new QMediaPlayer();
       sound->setMedia(QUrl("C:/Users/Asus/Documents/cine/click3.mp3"));
       sound->play();
    if (ui->checkBox_numpTri->isChecked())
    {
        ui->tableView_projections->setModel(tmpProjection.trier_p("num_projection"));
    }
    else if(ui->checkBox_datePTri->isChecked())
    {
        ui->tableView_projections->setModel(tmpProjection.trier_p("date_projection"));
    }
    else if(ui->checkBox_salleTri->isChecked())
    {
        ui->tableView_projections->setModel(tmpProjection.trier_p("num_salle"));
    }
}


void MainWindow::on_exporterPdf_proj_clicked()
{
    QMediaPlayer * sound = new QMediaPlayer();
       sound->setMedia(QUrl("C:/Users/Asus/Documents/cine/click3.mp3"));
       sound->play();
    QString strStream;
            QTextStream out(&strStream);
            const int rowCount = ui->tableView_projections->model()->rowCount();
            const int columnCount =ui->tableView_projections->model()->columnCount();

            out <<  "<html>\n"
                    "<head>\n"
                    "<meta Content=\"Text/html; charset=Windows-1251\">\n"
                    <<  QString("<title>%1</title>\n").arg("projection")
                    <<  "</head>\n"
                    "<body bgcolor=#C5D4F6 link=#5000A0>\n"
                        "<img src='C:/Users/Asus/Documents/cine/hexa_script.png' width='100' height='100'>\n"
                        "<h1>Liste des projections</h1>"
                        "<table border=1 cellspacing=0 cellpadding=2>\n";

            // headers
                out << "<thead><tr bgcolor=#f0f0f0>";
                for (int column = 0; column < columnCount; column++)
                    if (!ui->tableView_projections->isColumnHidden(column))
                        out << QString("<th>%1</th>").arg(ui->tableView_projections->model()->headerData(column, Qt::Horizontal).toString());
                out << "</tr></thead>\n";
                // data table
                   for (int row = 0; row < rowCount; row++) {
                       out << "<tr>";
                       for (int column = 0; column < columnCount; column++) {
                           if (!ui->tableView_projections->isColumnHidden(column)) {
                               QString data = ui->tableView_projections->model()->data(ui->tableView_projections->model()->index(row, column)).toString().simplified();
                               out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                           }
                       }
                       out << "</tr>\n";
                   }
                   out <<  "</table>\n"
                       "</body>\n"
                       "</html>\n";

                   QTextDocument *document = new QTextDocument();
                   document->setHtml(strStream);
                   QPrinter printer;
                   QPrintDialog *dialog = new QPrintDialog(&printer, NULL);
                   if (dialog->exec() == QDialog::Accepted) {
                       document->print(&printer);
                   }
}

void MainWindow::on_exporterExcel_proj_clicked()
{
    QMediaPlayer * sound = new QMediaPlayer();
       sound->setMedia(QUrl("C:/Users/Asus/Documents/cine/click3.mp3"));
       sound->play();
    tmpProjection.exporterExcel_p(ui->tableView_projections);
}


void MainWindow::on_pushButton_afficherF_clicked()
{
    QMediaPlayer * sound = new QMediaPlayer();
       sound->setMedia(QUrl("C:/Users/Asus/Documents/cine/click3.mp3"));
       sound->play();
    ui->tableView_films->setModel(tmpFilm.afficher());
}

void MainWindow::on_pushButton_afficherP_clicked()
{
    QMediaPlayer * sound = new QMediaPlayer();
       sound->setMedia(QUrl("C:/Users/Asus/Documents/cine/click3.mp3"));
       sound->play();
    ui->tableView_projections->setModel(tmpProjection.afficher_p());
}

void MainWindow::on_nightmode_clicked()
{
    ui->groupBox_ajoutP->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 gray, stop: 0.3 gray, stop: 1 gray);}");
    ui->groupBox_listeP->setStyleSheet("background-color: gray}");
    ui->groupBox_ajoutF->setStyleSheet("background-color: gray}");
    ui->groupBox_listeF->setStyleSheet("background-color: gray}");
}

void MainWindow::on_daymode_clicked()
{
    ui->groupBox_ajoutF->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x1: 0, y2: 1, stop: 0.5 #88D1C7, stop: 1 #FFFFFF);}");
    ui->groupBox_listeF->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x1: 0, y2: 1, stop: 0 #88D1C7, stop: 1 #FFFFFF);}");
    ui->groupBox_ajoutP->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x1: 0, y2: 1, stop: 0 #88D1C7, stop: 1 #FFFFFF);}");
    ui->groupBox_listeP->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x1: 0, y2: 1, stop: 0 #88D1C7, stop: 1 #FFFFFF);}");
}

void MainWindow::on_returnPROG_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::update_label()
{
    data=A.read_from_arduino();
    tmp+=data;
    qDebug() << data;
    if (tmp.endsWith("\r\n"))
    {
        tmp = tmp.left(5);
        ui->lcdNumber_temp->display(tmp.toFloat());
        if(tmp.toFloat() > 20)
        {
            QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;
            notifyIcon->show();
            notifyIcon->setIcon(QIcon("icone.png"));
            notifyIcon->showMessage("GESTION NOTIF ","ATTENTION TEMPERATURE ELEVEE DETECTEE",QSystemTrayIcon::Information,15000);
         }
        tmp="";
     }
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

//----------ELI BECH Y'INTEGRI BA3DI--------//
