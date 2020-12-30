#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "connexion.h"
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    connexion C;
    bool test = C.createConnexion();
    MainWindow w;

    if (test)
    {
        w.show();
        QMessageBox::information(nullptr, QObject::tr("database is open") ,
                     QObject::tr("connection succsseful.\n" "Click cancel to exit"), QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("database is not open") ,
                     QObject::tr("connection failed.\n" "Click cancel to exit"), QMessageBox::Cancel);
    }

    QTranslator myappTranslator;
       myappTranslator.load(QLocale(), QLatin1String("myapp"), QLatin1String("_"), QLatin1String(":/i18n"));
       a.installTranslator(&myappTranslator);


    return a.exec();
}
