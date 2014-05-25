#include <QtWidgets/QApplication>
#include "FenServeur.h"


int main(int argc, char*argv[])
{
    QApplication app(argc, argv);


    FenServeur* fenetre = new FenServeur();

    fenetre->show();

    return app.exec();


}


