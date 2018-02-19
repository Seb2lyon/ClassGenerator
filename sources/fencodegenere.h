#ifndef FENCODEGENERE_H
#define FENCODEGENERE_H

#include <QtWidgets>
#include "fenprincipale.h"

class FenCodeGenere : public QDialog
{
    Q_OBJECT

    public:
        FenCodeGenere(FenPrincipale *fenetre, QString *chaineCodeH, QString *chaineCodeCPP);

    private:
};

#endif // FENCODEGENERE_H
