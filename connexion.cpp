#include "connexion.h"
#include "QSqlDatabase"


connexion::connexion()
{}

bool connexion::createConnexion()
{
    bool test=false;
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("test");
    db.setUserName("dorsaf");
    db.setPassword("dorsaf12");

    if (db.open())
        test = true;
    return test;

}
void connexion::closeConnexion()
{
    db.close();
}
