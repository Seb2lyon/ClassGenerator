#ifndef FENCODEGENERE_H
#define FENCODEGENERE_H

#include <QApplication>
#include <QtWidgets>
#include "fenprincipale.h"

class FenCodeGenere : public QDialog
{
    public:
        FenCodeGenere(FenPrincipale *fenetre, QString *chaineCode);

    private:
};

#endif // FENCODEGENERE_H
