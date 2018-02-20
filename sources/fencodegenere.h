#ifndef FENCODEGENERE_H
#define FENCODEGENERE_H

#include <QtWidgets>
#include "fenprincipale.h"

class FenCodeGenere : public QDialog
{
    Q_OBJECT

    public:
        FenCodeGenere(FenPrincipale *fenetre, QString *chaineCodeH, QString *chaineCodeCPP);

    public slots:
        void enregistrerCodes();

    private:
        FenPrincipale *copieFenPrincipale;
        QString nomPageH;
        QString nomPageCPP;
        QString *enregistreCodeH;
        QString *enregistreCodeCPP;
};

#endif // FENCODEGENERE_H
