#include "FenServeur.h"
#include <QVBoxLayout>

FenServeur::FenServeur()
{
    etatServeur = new QLabel;
    boutonQuitter = new QPushButton("Quitter");

    connect(boutonQuitter, SIGNAL(clicked()), qApp, SLOT(quit()));

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(etatServeur);
    layout->addWidget(boutonQuitter);
    setLayout(layout);

    setWindowTitle("YassineChat - Server");

    serveur = new QTcpServer(this);

    if(!serveur->listen(QHostAddress::Any, 50885))
    {
        etatServeur->setText("Le serveur n'as pas pu etre demarre. <br/>"+serveur->errorString());
    }
    else
    {
        etatServeur->setText("Le serveur a demarrer sur le port <strong>" + QString::number(serveur->serverPort()) + "</strong>.<br/> Des clients peuvent maintenant se connecter");
        connect(serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));

    }
    tailleMessage = 0;
}

void FenServeur::nouvelleConnexion()
{
    envoyerATous("<em>Un nouveau Client vient de se connecter</em>");

    QTcpSocket* nouveauClient = serveur->nextPendingConnection();
    clients << nouveauClient;

    connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
    connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(deconnexionClient()));

}


void FenServeur::donneesRecues()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if(socket == 0)
        return;

    QDataStream in(socket);

    if(tailleMessage == 0)
    {
        if(socket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> tailleMessage;
    }

    if(socket->bytesAvailable() < tailleMessage)
    {
        return;
    }

    QString message;
    in >> message;

    envoyerATous(message);
    tailleMessage = 0;
}


void FenServeur::deconnexionClient()
{
    envoyerATous("Un client vient de se deconnecter");

    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if(socket == 0)
        return;

    clients.removeOne(socket);
    socket->deleteLater();
}

void FenServeur::envoyerATous(const QString &message)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
     out << (quint16) 0; // On écrit 0 au début du paquet pour
    //réserver la place pour écrire la taille
     out << message; // On ajoute le message à la suite
     out.device()->seek(0); // On se replace au début du paquet
     out << (quint16) (paquet.size() - sizeof(quint16)); // On écrase
    //le 0 qu'on avait réservé par la longueur du message
     // Envoi du paquet préparé à tous les clients connectés au
    //serveur
     for (int i = 0; i < clients.size(); i++)
     {
         clients[i]->write(paquet);
     }
}

FenServeur::~FenServeur()
{
    delete etatServeur;
    delete boutonQuitter;
    delete serveur;
    //delete[] clients;

}

