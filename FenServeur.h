#ifndef FENSERVEUR_H
#define FENSERVEUR_H

#include <QtGui>
#include <QtNetwork>
#include <QLabel>
#include <QPushButton>

class FenServeur : public QWidget
{
    Q_OBJECT

public:

    FenServeur();
    ~FenServeur();
    void envoyerATous(const QString &message);

private slots:

    void nouvelleConnexion();
    void donneesRecues();
    void deconnexionClient();

private:

    QLabel* etatServeur;
    QPushButton* boutonQuitter;
    QTcpServer* serveur;
    QList<QTcpSocket *>clients;
    quint16 tailleMessage;
};




#endif // FENSERVEUR_H
