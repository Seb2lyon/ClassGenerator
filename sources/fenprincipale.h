#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#include <QApplication>
#include <QtWidgets>

class FenPrincipale : public QWidget
{
    Q_OBJECT

    public :
        FenPrincipale();

    public slots :
        void validerFenPrincipale();

    private :
        QLineEdit *nomClasse; // Class name (should be set)
        QLineEdit *nomClasseMere; // Mother-class name
        QCheckBox *protegeHeader; // Checkbox "Protect header against multiple inclusions"
        QCheckBox *genereConstructeur; // Checkbox "Generate default constructor"
        QCheckBox *genereDestructeur; // Checkbox "Generate destructor"
        QGroupBox *ajoutCommentaires; // Checkbox "Add comments"
        QLineEdit *auteur; // Athor name
        QDateEdit *dateCreation; // Creation date
        QTextEdit *roleClasse; // Class description
};

#endif // FENPRINCIPALE_H
