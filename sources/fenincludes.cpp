#include "fenincludes.h"

using namespace std;

FenIncludes::FenIncludes(FenPrincipale *fenetre, int nbrIncludesActifs, vector<QString>listeIncludesActifs, int nbrIncludesDispo, vector<QString>listeIncludesDispo) : QDialog(fenetre, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    setWhatsThis(QString(tr("Choisissez les \"includes\" que vous voulez faire apparaitre dans votre code")));

    // Create all the copies
    copieFenPrincipale = fenetre;
    copieListeIncludesActifs = listeIncludesActifs;
    copieListeIncludesDispo = listeIncludesDispo;
    includesActifs = new QListWidget;
    includesDispo = new QListWidget;
    includesDispo->setSortingEnabled(true);

    for(int i = 0; i < nbrIncludesActifs; ++i)
    {
        includesActifs->addItem(copieListeIncludesActifs[i]);
    }

    for(int i = 0; i < nbrIncludesDispo; ++i)
    {
        includesDispo->addItem(copieListeIncludesDispo[i]);
    }

    setFixedSize(450, 450);

    QLabel *titre = new QLabel(tr("Sélectionnez les \"includes\" de la bibliothèque standard C++ que vous souhaitez utiliser : "));

    QPushButton *boutonAjouter = new QPushButton(tr("Ajouter >>"));
    boutonAjouter->setFixedHeight(25);
    QPushButton *boutonSupprimer = new QPushButton(tr("<< Supprimer"));
    boutonSupprimer->setFixedHeight(25);
    QPushButton *boutonAnnuler = new QPushButton(tr("Annuler"));
    boutonAnnuler->setFixedHeight(25);
    QPushButton *boutonValider = new QPushButton(tr("VALIDER"));
    QFont font;
    font.setBold(true);
    boutonValider->setFont(font);
    boutonValider->setFixedHeight(25);

    QVBoxLayout * layoutBoutons = new QVBoxLayout;
    layoutBoutons->addWidget(boutonAjouter);
    layoutBoutons->addWidget(boutonSupprimer);
    layoutBoutons->addWidget(boutonAnnuler);

    QHBoxLayout *layoutIncludes = new QHBoxLayout;
    layoutIncludes->addWidget(includesDispo);
    layoutIncludes->addLayout(layoutBoutons);
    layoutIncludes->addWidget(includesActifs);

    QVBoxLayout *layoutGlobal = new QVBoxLayout;
    layoutGlobal->addWidget(titre);
    layoutGlobal->addLayout(layoutIncludes);
    layoutGlobal->addWidget(boutonValider);

    setLayout(layoutGlobal);

    QObject::connect(boutonAjouter, SIGNAL(clicked()), this, SLOT(ajouterIncludes()));
    QObject::connect(includesDispo, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ajouterIncludes()));
    QObject::connect(boutonSupprimer, SIGNAL(clicked()), this, SLOT(supprimerIncludes()));
    QObject::connect(includesActifs, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(supprimerIncludes()));
    QObject::connect(boutonAnnuler, SIGNAL(clicked()), this, SLOT(annulerIncludes()));
    QObject::connect(boutonValider, SIGNAL(clicked()), this, SLOT(validerIncludes()));
    QObject::connect(this, SIGNAL(rejected()), this, SLOT(rien()));

}

// Add includes
void FenIncludes::ajouterIncludes()
{
    if(includesDispo->count() != 0 || !includesDispo->selectedItems().isEmpty())
    {
        includesActifs->addItem(includesDispo->currentItem()->text());
        includesDispo->takeItem(includesDispo->currentIndex().row());
    }
}

// Cancel includes
void FenIncludes::supprimerIncludes()
{
    if(includesActifs->count() != 0 || !includesActifs->selectedItems().isEmpty())
    {
        includesDispo->addItem(includesActifs->currentItem()->text());
        includesActifs->takeItem(includesActifs->currentIndex().row());
    }
}

// Cancel all includes
void FenIncludes::annulerIncludes()
{
    if(includesActifs->count() != 0)
    {
        for(int i = 0; i < includesActifs->count(); ++i)
        {
            includesDispo->addItem(includesActifs->item(i)->text());
        }

        includesActifs->clear();
    }
}

// Validate Includes window
void FenIncludes::validerIncludes()
{
    if(includesActifs->count() == 0)
    {
        copieFenPrincipale->getAjoutIncludes()->setCheckState(Qt::Unchecked);
    }

    int nbrIncludesDispo = includesDispo->count();
    int nbrIncludesActifs = includesActifs->count();

    vector<QString>copieListeIncludesActifs;
    vector<QString>copieListeIncludesDispo;

    for(int i = 0; i < nbrIncludesActifs; ++i)
    {
        copieListeIncludesActifs.push_back(includesActifs->item(i)->text());
    }

    for(int i = 0; i < nbrIncludesDispo; ++i)
    {
        copieListeIncludesDispo.push_back(includesDispo->item(i)->text());
    }

    copieFenPrincipale->setIncludesDispo(copieListeIncludesDispo, nbrIncludesDispo);
    copieFenPrincipale->setIncludesActifs(copieListeIncludesActifs, nbrIncludesActifs);

    this->accept();
}

// Manage escape and cross close window
void FenIncludes::rien()
{
    if(copieListeIncludesActifs.size() == 0)
    {
        copieFenPrincipale->getAjoutIncludes()->setCheckState(Qt::Unchecked);
    }

    this->accept();
}
