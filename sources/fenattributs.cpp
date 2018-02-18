#include "fenattributs.h"

using namespace std;

FenAttributs::FenAttributs(FenPrincipale *fenetre, int nbrAttributs, vector<Attribut> tableauAttributs) : QDialog(fenetre, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    // Create all the copies
    copieFenPrincipale = fenetre;
    copieNbrAttributs = nbrAttributs;
    copieListeAttributs = tableauAttributs;

    setFixedSize(450, 450);

    QValidator *validator = new QRegularExpressionValidator(QRegularExpression("[_A-Za-z0-9]{0,32}"));

    // Three groups of global layout
    QGroupBox *optionsAttribut = new QGroupBox(tr("Options de l'attribut"));
    QGroupBox *listeAttributs = new QGroupBox(tr("Liste des attributs"));
    QPushButton *valider = new QPushButton(tr("VALIDER"));
    valider->setFixedHeight(25);
    QFont font;
    font.setBold(true);
    valider->setFont(font);

    // Form creating attribute
    listeTypes = new QComboBox;
    listeTypes->addItem("bool");
    listeTypes->addItem("char");
    listeTypes->addItem("double");
    listeTypes->addItem("float");
    listeTypes->addItem("int");
    listeTypes->addItem("string");
    listeTypes->addItem("void");
    listeTypes->addItem("wchar_t");
    nomAttribut = new QLineEdit;
    nomAttribut->setValidator(validator);
    nomAttribut->setStyleSheet("background: rgb(255,255,255)");
    pointeur = new QCheckBox;
    vectorAttribut = new QCheckBox;
    privateAttribut = new QRadioButton;
    privateAttribut->setText("pr&ivate");
    privateAttribut->setChecked(true);
    protectedAttribut = new QRadioButton;
    protectedAttribut->setText("pr&otected");

    QFormLayout *formulaire = new QFormLayout;
    formulaire->addRow(tr("&Type : "), listeTypes);
    formulaire->addRow(tr("&Nom : "), nomAttribut);
    formulaire->addRow(tr("&Pointeur : "), pointeur);
    formulaire->addRow(tr("&Vector : "), vectorAttribut);
    formulaire->addRow(tr("Visibilité : "), privateAttribut);
    formulaire->addRow("", protectedAttribut);

    // Cancel / Add buttons
    QHBoxLayout *annulerAjouter = new QHBoxLayout;
    QPushButton *annuler = new QPushButton(tr("Annuler"));
    annuler->setFixedHeight(25);
    ajouter = new QPushButton;
    ajouter->setText(tr("Ajouter"));
    ajouter->setFixedHeight(25);
    annuler->setAutoDefault(false);
    annulerAjouter->addWidget(annuler);
    annulerAjouter->addWidget(ajouter);

    // First Group Box
    QVBoxLayout *premierLayout = new QVBoxLayout;
    premierLayout->addLayout(formulaire);
    premierLayout->addLayout(annulerAjouter);

    optionsAttribut->setLayout(premierLayout);

    // List of attributes created
    listeAttributsCrees = new QListWidget;

    for(int i = 0; i < copieNbrAttributs; ++i)
    {
        QString attributFormate;
        if(copieListeAttributs[i].vector)
        {
            attributFormate.append("vector<");
            attributFormate.append(copieListeAttributs[i].type);
            attributFormate.append(">");
        }
        else
        {
            attributFormate.append(copieListeAttributs[i].type);
        }
        attributFormate.append(" ");
        if(copieListeAttributs[i].pointeur == true)
        {
            attributFormate.append("*");
        }
        attributFormate.append(copieListeAttributs[i].nom);

        listeAttributsCrees->addItem(attributFormate);

        QString visibilite;
        if(copieListeAttributs[i].privateAttribut == true)
        {
            visibilite = "private";
        }
        else
        {
            visibilite = "protected";
        }

        listeAttributsCrees->item(i)->setToolTip(visibilite);
    }

    QVBoxLayout *modifierSupprimer = new QVBoxLayout;

    modifier = new QPushButton(tr("Modifier"));
    modifier->setFixedHeight(25);
    supprimer = new QPushButton(tr("Supprimer"));
    supprimer->setFixedHeight(25);
    toutSupprimer = new QPushButton(tr("Tout supprimer"));
    toutSupprimer->setFixedHeight(25);
    toutSupprimer->setFixedWidth(90);

    // If there's no attributes in the list
    if(listeAttributsCrees->count() == 0)
    {
        modifier->setEnabled(false);
        supprimer->setEnabled(false);
        toutSupprimer->setEnabled(false);
    }
    else if(listeAttributsCrees->selectedItems().isEmpty())
    {
        modifier->setEnabled(false);
        supprimer->setEnabled(false);
    }

    modifierSupprimer->addWidget(modifier);
    modifierSupprimer->addWidget(supprimer);
    modifierSupprimer->addWidget(toutSupprimer);

    // Second Group Box
    QHBoxLayout *deuxiemeLayout = new QHBoxLayout;
    deuxiemeLayout->addWidget(listeAttributsCrees);
    deuxiemeLayout->addLayout(modifierSupprimer);

    listeAttributs->setLayout(deuxiemeLayout);

    // Global Layout
    QVBoxLayout *generalLayout = new QVBoxLayout;
    generalLayout->addWidget(optionsAttribut);
    generalLayout->addWidget(listeAttributs);
    generalLayout->addWidget(valider);

    setLayout(generalLayout);

    QObject::connect(annuler, SIGNAL(clicked()), this, SLOT(annulerNvAttribut()));
    QObject::connect(ajouter, SIGNAL(clicked()), this, SLOT(ajouterNvAttribut()));
    QObject::connect(listeAttributsCrees, SIGNAL(itemSelectionChanged()), this, SLOT(selectionAttribut()));
    QObject::connect(modifier, SIGNAL(clicked()), this, SLOT(modificationAttribut()));
    QObject::connect(supprimer, SIGNAL(clicked()), this, SLOT(suppressionAttribut()));
    QObject::connect(toutSupprimer, SIGNAL(clicked()), this, SLOT(suppressionTousAttributs()));
    QObject::connect(valider, SIGNAL(clicked()), this, SLOT(validerFenAttributs()));
    QObject::connect(this, SIGNAL(rejected()), this, SLOT(rien()));
}

// Clear form
void FenAttributs::annulerNvAttribut()
{
    nomAttribut->setStyleSheet("background: rgb(255,255,255)");
    listeTypes->setCurrentIndex(0);
    nomAttribut->setText("");
    pointeur->setChecked(false);
    vectorAttribut->setChecked(false);
    privateAttribut->setChecked(true);
    ajouter->setText(tr("Ajouter"));
}

// Add new attribute
void FenAttributs::ajouterNvAttribut()
{
    QValidator *validator = new QRegularExpressionValidator(QRegularExpression("[_A-Za-z][_A-Za-z0-9]{0,31}"));
    int pos = 0;
    QString inputAttribut(nomAttribut->text());

    // If attribute has no name
    if(nomAttribut->text().isEmpty())
    {
        nomAttribut->setStyleSheet("background: rgb(252,254,171)");
        nomAttribut->setFocus(Qt::OtherFocusReason);
        QMessageBox::warning(this, tr("Attention"), tr("Vous devez renseigner le nom de l'attribut"));
    }
    // If attribute's name is not valid
    else if(validator->validate(inputAttribut, pos) != QValidator::Acceptable)
    {
        nomAttribut->setStyleSheet("background: rgb(252,254,171)");
        nomAttribut->setFocus(Qt::OtherFocusReason);
        QMessageBox::warning(this, tr("Attention"), tr("Le nom de l'attribut est invalide :\nLe nom ne doit pas commencer par un chiffre"));
    }
    // If it's OK
    else
    {
        int nombreAttributs = listeAttributsCrees->count();
        bool dejaPris(false);

        for(int i = 0; i < nombreAttributs; ++i)
        {
            if(copieListeAttributs[i].nom == nomAttribut->text())
            {
                dejaPris = true;
            }
        }

        // If the name is already taken
        if(dejaPris && (ajouter->text() == QString(tr("Ajouter")) || (ajouter->text() == QString(tr("Modifier")) && nomAttribut->text() != copieListeAttributs[positionAttribut].nom)))
        {
            nomAttribut->setStyleSheet("background: rgb(252,254,171)");
            nomAttribut->setFocus(Qt::OtherFocusReason);
            QMessageBox::warning(this, tr("Attention"), tr("Le nom de l'attribut est déjà pris\nVeuillez en saisir un nouveau"));
        }
        else
        {
            nomAttribut->setStyleSheet("background: rgb(255,255,255)");
            Attribut attribut;
            attribut.type = listeTypes->currentText();
            attribut.nom = nomAttribut->text();
            attribut.pointeur = pointeur->isChecked();
            attribut.vector = vectorAttribut->isChecked();
            attribut.privateAttribut = privateAttribut->isChecked();

            QString attributFormate;
            if(attribut.vector)
            {
                attributFormate.append("vector<");
                attributFormate.append(attribut.type);
                attributFormate.append(">");
            }
            else
            {
                attributFormate.append(attribut.type);
            }
            attributFormate.append(" ");
            if(attribut.pointeur == true)
            {
                attributFormate.append("*");
            }
            attributFormate.append(attribut.nom);

            // If this is a modification
            if(ajouter->text() == QString(tr("Modifier")))
            {
                listeAttributsCrees->takeItem(positionAttribut);
                listeAttributsCrees->insertItem(positionAttribut, attributFormate);
                copieListeAttributs[positionAttribut] = attribut;
                listeAttributsCrees->setCurrentRow(positionAttribut);
            }
            // If this is a creation
            else
            {
                listeAttributsCrees->addItem(attributFormate);
                copieListeAttributs.push_back(attribut);
            }

            toutSupprimer->setEnabled(true);

            annulerNvAttribut();

            nombreAttributs = listeAttributsCrees->count();

            // Show the Tool Tip
            for(int i = 0; i < nombreAttributs; ++i)
            {
                QString visibilite;

                if(copieListeAttributs[i].privateAttribut == true)
                {
                    visibilite = "private";
                }
                else
                {
                    visibilite = "protected";
                }

                listeAttributsCrees->item(i)->setToolTip(visibilite);
            }
        }
    }
}

// Select attribute in the list
void FenAttributs::selectionAttribut()
{
    if(!listeAttributsCrees->selectedItems().isEmpty())
    {
        modifier->setEnabled(true);
        supprimer->setEnabled(true);
    }
}

// Modify selected attribute
void FenAttributs::modificationAttribut()
{
    nomAttribut->setStyleSheet("background: rgb(255,255,255)");

    positionAttribut = listeAttributsCrees->currentRow();

    Attribut attribut;
    attribut = copieListeAttributs[positionAttribut];

    nomAttribut->setText(attribut.nom);
    listeTypes->setCurrentText(attribut.type);
    pointeur->setChecked(attribut.pointeur);
    vectorAttribut->setChecked(attribut.vector);
    if(attribut.privateAttribut == true)
    {
        privateAttribut->setChecked(true);
    }
    else
    {
        protectedAttribut->setChecked(true);
    }

    ajouter->setText(tr("Modifier"));
}

// Delete selected attribute
void FenAttributs::suppressionAttribut()
{
    if(listeAttributsCrees->count() != 0)
    {
        positionAttribut = listeAttributsCrees->currentRow();

        nomAttribut->setStyleSheet("background: rgb(255,255,255)");

        listeAttributsCrees->takeItem(positionAttribut);
        copieListeAttributs.erase(copieListeAttributs.begin() + positionAttribut);

        annulerNvAttribut();

        if(listeAttributsCrees->count() == 0)
        {
            modifier->setEnabled(false);
            supprimer->setEnabled(false);
            toutSupprimer->setEnabled(false);
        }
        else if(listeAttributsCrees->selectedItems().isEmpty())
        {
            modifier->setEnabled(false);
            supprimer->setEnabled(false);
        }
    }
}

// Cancel all the attributes in the list
void FenAttributs::suppressionTousAttributs()
{
    if(listeAttributsCrees->count() != 0)
    {
        nomAttribut->setStyleSheet("background: rgb(255,255,255)");
        listeAttributsCrees->clear();
        copieListeAttributs.clear();
        annulerNvAttribut();
        modifier->setEnabled(false);
        supprimer->setEnabled(false);
        toutSupprimer->setEnabled(false);
    }
}

// Validate attribute's window
void FenAttributs::validerFenAttributs()
{
    if(listeAttributsCrees->count() == 0)
    {
        copieFenPrincipale->getAjoutAttributs()->setCheckState(Qt::Unchecked);
        copieFenPrincipale->getGenereAccesseurs()->setEnabled(false);
        copieFenPrincipale->getGenereAccesseurs()->setChecked(false);
    }
    else
    {
        copieFenPrincipale->getGenereAccesseurs()->setEnabled(true);
    }

    copieFenPrincipale->setAttributs(copieListeAttributs, listeAttributsCrees->count());

    this->accept();
}

// Manage escape and cross close window
void FenAttributs::rien()
{
    if(copieFenPrincipale->getListeAttributs().size() == 0)
    {
        copieFenPrincipale->getAjoutAttributs()->setCheckState(Qt::Unchecked);
        copieFenPrincipale->getGenereAccesseurs()->setEnabled(false);
        copieFenPrincipale->getGenereAccesseurs()->setChecked(false);
    }
    else
    {
        copieFenPrincipale->getGenereAccesseurs()->setEnabled(true);
    }

    this->accept();
}
