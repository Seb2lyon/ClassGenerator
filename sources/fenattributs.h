#ifndef FENATTRIBUTS_H
#define FENATTRIBUTS_H

#include <QtWidgets>
#include <vector>
#include "fenprincipale.h"

// Structure Attribute
struct Attribut
{
    QString type;
    QString nom;
    bool pointeur;
    bool vector;
    bool privateAttribut;
};

// Class of the attributes' window
class FenAttributs : public QDialog
{
    Q_OBJECT

    public:
        FenAttributs(FenPrincipale *fenetre, int nbrAttributs, std::vector<Attribut> listeAttributs);

    public slots:
        void ajouterNvAttribut();
        void annulerNvAttribut();
        void selectionAttribut();
        void modificationAttribut();
        void suppressionAttribut();
        void suppressionTousAttributs();
        void validerFenAttributs();
        void rien();

    private:
        FenPrincipale *copieFenPrincipale; // Copy of the main windows
        QComboBox *listeTypes; // Liste of attribute's type
        QLineEdit *nomAttribut; // Name of the attribute
        QCheckBox *pointeur; // Checkbox pointer
        QCheckBox *vectorAttribut; // Checkbox vector
        QRadioButton *privateAttribut; // Radio button private attribute
        QRadioButton *protectedAttribut; // Radio button protected attribute
        QPushButton *ajouter; // Add button
        QListWidget *listeAttributsCrees; // List of the created attributes
        int copieNbrAttributs; // Number of attributes created
        std::vector<Attribut> copieListeAttributs; // Array of the created attributes
        QPushButton *modifier; // Modify button
        int positionAttribut; // Row of the attribute to modify
        QPushButton *supprimer; // Delete button
        QPushButton *toutSupprimer; // Delete all button
};

#endif // FENATTRIBUTS_H
