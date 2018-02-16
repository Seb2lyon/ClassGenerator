#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#include <QApplication>
#include <QtWidgets>
#include <vector>
#include "fenattributs.h"

class FenPrincipale : public QWidget
{
    Q_OBJECT

    public :
        FenPrincipale();
        QCheckBox *getAjoutIncludes();
        QCheckBox *getAjoutAttributs();
        QCheckBox *getGenereAccesseurs();
        void setIncludesDispo(std::vector<QString> nouvelleListeIncludesDispo, int copieNbrIncludesDispo);
        void setIncludesActifs(std::vector<QString> nouvelleListeIncludesActifs, int copieNbrIncludesActifs);
        void setAttributs(std::vector<Attribut> nouvelleListeAttributs, int copieNbrAttributs);

    public slots :
        void validerFenPrincipale();
        void activeHeaderGuard();
        void genereHeaderGuard();
        void gestionIncludes();
        void gestionAttributs();
        void fenetreInfo();

    private :
        QLineEdit *nomClasse; // Class name (should be set)
        QLineEdit *nomClasseMere; // Mother-class name
        QCheckBox *protegeHeader; // Checkbox "Protect header against multiple inclusions"
        QLineEdit *headerGuard; // Edit Header Guard
        QCheckBox *genereConstructeur; // Checkbox "Generate default constructor"
        QCheckBox *genereDestructeur; // Checkbox "Generate destructor"
        QCheckBox *ajoutIncludes; // Checkbox "Add includes"
        int nbrIncludesDispo; // Number of available includes
        std::vector<QString> listeIncludesDispo; // Array of available includes
        int nbrIncludesActifs; // Number of selected includes
        std::vector<QString> listeIncludesActifs; // Array of selected includes
        QCheckBox *ajoutAttributs; // CheckBox "Add attibutes"
        int nbrAttributs; // Number of attributes created
        std::vector<Attribut> listeAttributs; // Array of attributes created
        QCheckBox *genereAccesseurs; // Checkbox "Generate accessors"
        QGroupBox *ajoutCommentaires; // Checkbox "Add comments"
        QLineEdit *auteur; // Author name
        QDateEdit *dateCreation; // Creation date
        QComboBox *license; // Choose license
        QTextEdit *roleClasse; // Class description
};

#endif // FENPRINCIPALE_H
