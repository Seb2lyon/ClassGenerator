#ifndef FENINCLUDES_H
#define FENINCLUDES_H

#include <QtWidgets>
#include <vector>
#include "fenprincipale.h"

class FenIncludes : public QDialog
{
    Q_OBJECT

    public:
        FenIncludes(FenPrincipale *fenetre, int nbrIncludesActifs, std::vector<QString>listeIncludesActifs, int nbrIncludesDispo, std::vector<QString>listeIncludesDispo);

    public slots:
        void ajouterIncludes();
        void supprimerIncludes();
        void annulerIncludes();
        void validerIncludes();
        void rien();

    private:
        FenPrincipale *copieFenPrincipale; // Copy of the main windows
        std::vector<QString>copieListeIncludesActifs; // Copy of the array of selected includes
        std::vector<QString>copieListeIncludesDispo; // Copy of the array of available includes
        QListWidget *includesActifs; // Copy of the selected includes' list
        QListWidget *includesDispo; // Copy of the available includes's list
};

#endif // FENINCLUDES_H
