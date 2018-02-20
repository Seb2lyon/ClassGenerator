#include "fenprincipale.h"
#include "fencodegenere.h"
#include "fenincludes.h"
#include "fenattributs.h"
#include <fstream>
#include <string>

using namespace std;

FenPrincipale::FenPrincipale():QWidget()
{
    setFixedSize(450, 550);

    QValidator *validator = new QRegularExpressionValidator(QRegularExpression("[_A-Za-z0-9]{0,32}"));

    // 1) Form "Class definition"
    nomClasse = new QLineEdit;
    nomClasse->setStyleSheet("background: rgb(255,255,255)");
    nomClasse->setValidator(validator);
    nomClasseMere = new QLineEdit;
    nomClasseMere->setStyleSheet("background: rgb(255,255,255)");
    nomClasseMere->setValidator(validator);

    // Form Layout
    QFormLayout *defClasseLayout = new QFormLayout;
    defClasseLayout->addRow(tr("&Nom : "), nomClasse);
    defClasseLayout->addRow(tr("Classe &mère : "), nomClasseMere);

    // Set the form layout into QGroupBox
    QGroupBox *defClasse = new QGroupBox(tr("Définition de la classe"));
    defClasse->setLayout(defClasseLayout);


    // 2) Checkbox "Options"
    protegeHeader = new QCheckBox;
    protegeHeader->setText(tr("Protéger le &header contre les inclusions multiples"));
    protegeHeader->setChecked(true);
    headerGuard = new QLineEdit;
    genereConstructeur = new QCheckBox;
    genereConstructeur->setText(tr("Générer un &constructeur par défaut"));
    genereConstructeur->setChecked(true);
    genereDestructeur = new QCheckBox;
    genereDestructeur->setText(tr("Générer un &destructeur"));
    ajoutIncludes = new QCheckBox;
    ajoutIncludes->setText(tr("Ajouter des &includes"));
    ajoutAttributs = new QCheckBox;
    ajoutAttributs->setText(tr("Ajouter des &attributs"));
    genereAccesseurs = new QCheckBox;
    genereAccesseurs->setText(tr("&Générer accesseurs"));
    if(!ajoutAttributs->isChecked())
    {
        genereAccesseurs->setEnabled(false);
    }

    // Form Layout
    QFormLayout *optionsLayout = new QFormLayout;
    optionsLayout->addRow(protegeHeader);
    optionsLayout->addRow(headerGuard);
    optionsLayout->addRow(genereConstructeur);
    optionsLayout->addRow(genereDestructeur);
    optionsLayout->addRow(ajoutIncludes);
    optionsLayout->addRow(ajoutAttributs);
    optionsLayout->addRow(genereAccesseurs);

    // Set the form layout into QGroupBox
    QGroupBox *options = new QGroupBox(tr("Options"));
    options->setLayout(optionsLayout);


    // 3) Form "Add comments"
    auteur = new QLineEdit;
    dateCreation = new QDateEdit(QDate::currentDate());
    license = new QComboBox;
    license->addItem(tr("Aucune licence associée à ce logiciel"));
    license->addItem("Apache License 2.0");
    license->addItem("GNU GPLv3");
    license->addItem("MIT License");
    license->addItem(tr("Autre... (modifiez cette ligne)"));
    license->setEditable(true);
    roleClasse = new QTextEdit;

    // Form Layout
    QFormLayout *ajoutCommentairesLayout = new QFormLayout;
    ajoutCommentairesLayout->addRow(tr("A&uteur : "), auteur);
    ajoutCommentairesLayout->addRow(tr("Da&te de création : "), dateCreation);
    ajoutCommentairesLayout->addRow(tr("&Licence : "), license);
    ajoutCommentairesLayout->addRow(tr("&Rôle de la classe : "), roleClasse);

    // Set the form layout into QGroupBox
    ajoutCommentaires = new QGroupBox(tr("Ajouter des commentaires"));
    ajoutCommentaires->setCheckable(true);
    ajoutCommentaires->setChecked(false);
    ajoutCommentaires->setLayout(ajoutCommentairesLayout);


    // 4) Create buttons
    QPushButton *generer = new QPushButton(tr("Générer"));
    generer->setFixedSize(80, 25);
    QPushButton *quitter = new QPushButton(tr("Quitter"));
    quitter->setFixedSize(80, 25);

    // Buttons layout
    QHBoxLayout *boutons = new QHBoxLayout;
    boutons->addWidget(generer);
    boutons->addWidget(quitter);
    boutons->setAlignment(Qt::AlignRight);

    // Info button
    QPushButton *info = new QPushButton;
    info->setIcon(QIcon("images/info.png"));
    info->setFixedSize(QSize(25, 25));
    info->setIconSize(QSize(20, 20));
    info->setFlat(true);

    // End of page layout
    QHBoxLayout *piedpage = new QHBoxLayout;
    piedpage->addWidget(info);
    piedpage->addLayout(boutons);


    // Generate global layout
    QVBoxLayout *layoutGlobal = new QVBoxLayout;
    layoutGlobal->addWidget(defClasse);
    layoutGlobal->addWidget(options);
    layoutGlobal->addWidget(ajoutCommentaires);
    layoutGlobal->addLayout(piedpage);

    this->setLayout(layoutGlobal);


    // Includes lists (selected + available)

    ifstream fichier("config/includes.conf");

    string ligne;
    int occurences = 0;

    while(getline(fichier, ligne))
    {
        ligne = "<" + ligne + ">";
        listeIncludesDispo.push_back(QString::fromStdString(ligne));
        ++occurences;
    }

    // Number of includes
    nbrIncludesDispo = occurences;
    nbrIncludesActifs = 0;

    // Number of attributes
    nbrAttributs = 0;

    QObject::connect(quitter, SIGNAL(clicked()), qApp, SLOT(quit()));
    QObject::connect(generer, SIGNAL(clicked()), this, SLOT(validerFenPrincipale()));
    QObject::connect(protegeHeader, SIGNAL(stateChanged(int)), this, SLOT(activeHeaderGuard()));
    QObject::connect(nomClasse, SIGNAL(textChanged(QString)), this, SLOT(genereHeaderGuard()));
    QObject::connect(ajoutIncludes, SIGNAL(stateChanged(int)), this, SLOT(gestionIncludes()));
    QObject::connect(ajoutAttributs, SIGNAL(stateChanged(int)), this, SLOT(gestionAttributs()));
    QObject::connect(info, SIGNAL(clicked()), this, SLOT(fenetreInfo()));
}

// Validate main windows and prepare generated code windows
void FenPrincipale::validerFenPrincipale()
{
    QValidator *validator = new QRegularExpressionValidator(QRegularExpression("[_A-Za-z][_A-Za-z0-9]{0,31}"));
    int pos = 0;
    QString inputClass(nomClasse->text());
    QString inputClassMere(nomClasseMere->text());

    if(nomClasse->text().isEmpty())
    {
        nomClasse->setStyleSheet("background: rgb(252,254,171)");
        nomClasseMere->setStyleSheet("background: rgb(255,255,255)");
        nomClasse->setFocus(Qt::OtherFocusReason);
        QMessageBox::warning(this, tr("Attention"), tr("Vous devez renseigner le nom de la Classe"));
    }
    else if(validator->validate(inputClass, pos) != QValidator::Acceptable)
    {
        nomClasse->setStyleSheet("background: rgb(252,254,171)");
        nomClasseMere->setStyleSheet("background: rgb(255,255,255)");
        nomClasse->setFocus(Qt::OtherFocusReason);
        QMessageBox::warning(this, tr("Attention"), tr("Le nom de la Classe est invalide :\nLe nom ne doit pas commencer par un chiffre"));
    }
    else if(!nomClasseMere->text().isEmpty() && validator->validate(inputClassMere, pos) != QValidator::Acceptable)
    {
        nomClasse->setStyleSheet("background: rgb(255,255,255)");
        nomClasseMere->setStyleSheet("background: rgb(252,254,171)");
        nomClasseMere->setFocus(Qt::OtherFocusReason);
        QMessageBox::warning(this, tr("Attention"), tr("Le nom de la Classe mère est invalide :\nLe nom ne doit pas commencer par un chiffre"));
    }
    else if(nomClasseMere->text() == nomClasse->text())
    {
        nomClasse->setStyleSheet("background: rgb(255,255,255)");
        nomClasseMere->setStyleSheet("background: rgb(252,254,171)");
        nomClasseMere->setFocus(Qt::OtherFocusReason);
        QMessageBox::warning(this, tr("Attention"), tr("Le nom de la Classe mère est invalide :\nLe nom de la Classe mère doit être différent du nom de la Classe"));
    }
    else
    {
        nomClasse->setStyleSheet("background: rgb(255,255,255)");
        nomClasseMere->setStyleSheet("background: rgb(255,255,255)");


        // Generated code for H

        QString *chaineH = new QString;

        // Coments activated
        if(ajoutCommentaires->isChecked())
        {
            chaineH->append(QString("/*\n"));

            // If the author's name is set
            if(!auteur->text().isEmpty())
            {
                chaineH->append(QString(tr("Auteur : ")));
                chaineH->append(QString(auteur->text()));
                chaineH->append(QString("\n"));
            }

            chaineH->append(QString(tr("Date de création : ")));
            chaineH->append(QString(dateCreation->date().toString(Qt::SystemLocaleLongDate)));
            chaineH->append(QString("\n"));

            // If licence is set
            if(license->currentIndex() != 0 && license->currentText() != QString(tr("Autre... (modifiez cette ligne)")) && !license->currentText().isEmpty())
            {
                chaineH->append(tr("\nCe logiciel est sous licence : "));
                chaineH->append(license->currentText());
                chaineH->append("\n");
            }

            // If the role's text is written
            if(!roleClasse->toPlainText().isEmpty())
            {
                chaineH->append(QString(tr("\nRôle :\n")));
                chaineH->append(QString(roleClasse->toPlainText()));
                chaineH->append(QString("\n"));
            }

            chaineH->append(QString("*/\n\n\n"));
        }

        // Checkbox checked : Header protected
        if(protegeHeader->isChecked())
        {
            // If the user delete the generated Header Guard
            if(headerGuard->text().isEmpty())
            {
                chaineH->append(QString("#ifndef HEADER_"));
                chaineH->append(QString(nomClasse->text().toUpper()));
                chaineH->append(QString("\n#define HEADER_"));
                chaineH->append(QString(nomClasse->text().toUpper()));
            }
            else
            {
                chaineH->append(QString("#ifndef "));
                chaineH->append(QString(headerGuard->text().toUpper()));
                chaineH->append(QString("\n#define "));
                chaineH->append(QString(headerGuard->text().toUpper()));
            }

            chaineH->append(QString("\n\n\n"));
        }

        // Checkbox checked : Add includes
        bool includesExistants(false);

        if(ajoutIncludes->isChecked() && nbrIncludesActifs != 0)
        {
            includesExistants = true;

            for(int i = 0; i < nbrIncludesActifs; i++)
            {
                chaineH->append("#include ");
                chaineH->append(listeIncludesActifs[i]);
                chaineH->append("\n");
            }
        }

        // Add forced includes
        bool includeVector(false);
        bool attributVector(false);

        for(int i = 0; i < nbrIncludesActifs; ++i)
        {
            if(ajoutIncludes->isChecked() && listeIncludesActifs[i] == "<vector>")
            {
                includeVector = true;
            }
        }

        for(int i = 0; i < nbrAttributs; ++i)
        {
            if(ajoutAttributs->isChecked() && listeAttributs[i].vector == true)
            {
                attributVector = true;
            }
        }

        if(attributVector == true && includeVector == false)
        {
            chaineH->append(QString("#include <vector>\n"));
            includesExistants = true;
        }

        bool includeString(false);
        bool attributString(false);

        for(int i = 0; i < nbrIncludesActifs; ++i)
        {
            if(ajoutIncludes->isChecked() && listeIncludesActifs[i] == "<string>")
            {
                includeString = true;
            }
        }

        for(int i = 0; i < nbrAttributs; ++i)
        {
            if(ajoutAttributs->isChecked() && listeAttributs[i].type == "string")
            {
                attributString = true;
            }
        }

        if(attributString == true && includeString == false)
        {
            chaineH->append(QString("#include <string>\n"));
            includesExistants = true;
        }

        if(includesExistants)
        {
            chaineH->append(QString("\n\n"));
        }

        // Class set
        chaineH->append(QString("class "));
        chaineH->append(QString(nomClasse->text()));

        // Mother-class set
        if(!nomClasseMere->text().isEmpty())
        {
            chaineH->append(QString(" : public "));
            chaineH->append(QString(nomClasseMere->text()));
        }

        chaineH->append(QString("\n{\n    public:\n"));

        // Checkbox checked : Constructor generated
        if(genereConstructeur->isChecked())
        {
            chaineH->append(QString("        "));
            chaineH->append(QString(nomClasse->text()));
            chaineH->append(QString("();\n"));
        }

        // Checkbox checked : Destructor generated
        if(genereDestructeur->isChecked())
        {
            chaineH->append(QString("        ~"));
            chaineH->append(QString(nomClasse->text()));
            chaineH->append(QString("();\n"));
        }

        // Checkbox checked : Accessors generated
        if(genereAccesseurs->isChecked())
        {
            QString nouveauNom;

            if(nbrAttributs > 0)
            {
                for(int i = 0; i < nbrAttributs; ++i)
                {
                    chaineH->append(QString("        "));
                    if(listeAttributs[i].type == "string" || listeAttributs[i].vector == true)
                    {
                        chaineH->append(QString("std::"));
                    }
                    if(listeAttributs[i].vector == true)
                    {
                        chaineH->append(QString("vector<"));
                        if(listeAttributs[i].type == "string")
                        {
                            chaineH->append(QString("std::"));
                        }
                    }
                    chaineH->append(listeAttributs[i].type);
                    if(listeAttributs[i].vector == true)
                    {
                        chaineH->append(QString(">"));
                    }
                    chaineH->append(QString(" "));
                    if(listeAttributs[i].pointeur == true)
                    {
                        chaineH->append(QString("*"));
                    }
                    nouveauNom = listeAttributs[i].nom;
                    nouveauNom[0] = nouveauNom[0].toUpper();
                    chaineH->append(QString("get"));
                    chaineH->append(nouveauNom);
                    chaineH->append(QString("();\n"));
                }
            }
        }

        // Checkbox checked : Add attributes
        chaineH->append(QString("\n\n    protected:\n"));

        if(ajoutAttributs->isChecked())
        {
            chaineH->append(ajoutAttributsDansCode(false));
        }

        chaineH->append(QString("\n\n    private:\n"));

        if(ajoutAttributs->isChecked())
        {
            chaineH->append(ajoutAttributsDansCode(true));
        }

        chaineH->append(QString("\n\n};\n\n"));

        // End of generated code for H
        if(protegeHeader->isChecked())
        {
            chaineH->append(QString("#endif\n\n"));
        }


        // Generated code for CPP

        QString *chaineCPP = new QString;

        // Include class header
        chaineCPP->append(QString("#include \""));
        chaineCPP->append(nomClasse->text());
        chaineCPP->append(QString(".h\"\n\n\n"));

        // Class generator
        if(genereConstructeur->isChecked())
        {
            chaineCPP->append(QString(tr("// Constructeur\n")));
            chaineCPP->append(nomClasse->text());
            chaineCPP->append(QString("::"));
            chaineCPP->append(nomClasse->text());
            chaineCPP->append(QString("()"));

            if(!nomClasseMere->text().isEmpty())
            {
                chaineCPP->append(QString(" : "));
                chaineCPP->append(nomClasseMere->text());
                chaineCPP->append(QString("()"));
            }

            chaineCPP->append(QString("\n{\n\n}\n\n"));
        }

        // Class destructor
        if(genereDestructeur->isChecked())
        {
            bool pointeur(false);

            chaineCPP->append(QString(tr("// Destructeur\n")));
            chaineCPP->append(nomClasse->text());
            chaineCPP->append(QString("::~"));
            chaineCPP->append(nomClasse->text());
            chaineCPP->append(QString("()\n{\n"));
            for(int i = 0; i < nbrAttributs; ++i)
            {
                if(listeAttributs[i].pointeur == true)
                {
                    pointeur = true;
                    chaineCPP->append(QString("    delete "));
                    chaineCPP->append(listeAttributs[i].nom);
                    chaineCPP->append(QString(";\n"));
                }
            }

            if(pointeur == false)
            {
                chaineCPP->append(QString("\n"));
            }

            chaineCPP->append(QString("}\n\n"));
        }

        // Set accessors
        if(genereAccesseurs->isChecked())
        {
            chaineCPP->append(QString(tr("// Accesseurs\n")));

            for(int i = 0; i < nbrAttributs; ++i)
            {
                chaineCPP->append(nomClasse->text());
                chaineCPP->append(QString("::"));
                if(listeAttributs[i].vector == true)
                {
                    chaineCPP->append(QString("vector<"));
                }
                chaineCPP->append(listeAttributs[i].type);
                if(listeAttributs[i].vector == true)
                {
                    chaineCPP->append(QString(">"));
                }
                chaineCPP->append(QString(" "));
                if(listeAttributs[i].pointeur == true)
                {
                    chaineCPP->append(QString("*"));
                }

                QString nomModifie;
                nomModifie = listeAttributs[i].nom;
                nomModifie[0] = nomModifie[0].toUpper();

                chaineCPP->append(QString("get"));
                chaineCPP->append(nomModifie);
                chaineCPP->append("()\n{\n    return ");
                chaineCPP->append(listeAttributs[i].nom);
                chaineCPP->append(QString(";\n}\n\n"));
            }
        }

        // End of generated code for CPP

        FenCodeGenere fenetreCode(this, chaineH, chaineCPP);

        fenetreCode.exec();
    }
}

// Activate Header Guard line
void FenPrincipale::activeHeaderGuard()
{
    // Checkbox checked : Header protected
    if(protegeHeader->checkState() == Qt::Checked)
    {
        headerGuard->setEnabled(true);

        QString chaine("HEADER_");
        chaine.append(nomClasse->text().toUpper());

        if(nomClasse->text().isEmpty())
        {
            headerGuard->clear();
        }
        else
        {
            headerGuard->setText(chaine);
        }
    }
    else
    {
        headerGuard->clear();
        headerGuard->setEnabled(false);
    }
}

// Generate Header Guard
void FenPrincipale::genereHeaderGuard()
{
    // Checkbox checked : Header protected
    if(protegeHeader->isChecked())
    {
        QString chaine("HEADER_");
        chaine.append(nomClasse->text().toUpper());

        if(nomClasse->text().isEmpty())
        {
            headerGuard->clear();
        }
        else
        {
            headerGuard->setText(chaine);
        }
    }
}

// Manage Includes window
void FenPrincipale::gestionIncludes()
{
    if(ajoutIncludes->isChecked())
    {
        FenIncludes fenetreIncludes(this, nbrIncludesActifs, listeIncludesActifs, nbrIncludesDispo, listeIncludesDispo);
        fenetreIncludes.exec();
    }
}

// Manage Attributes window
void FenPrincipale::gestionAttributs()
{
    if(ajoutAttributs->isChecked())
    {
        FenAttributs fenetreAttributs(this, nbrAttributs, listeAttributs);
        fenetreAttributs.exec();
    }
    else
    {
        genereAccesseurs->setChecked(false);
        genereAccesseurs->setEnabled(false);
    }
}

// Information window
void FenPrincipale::fenetreInfo()
{
   QMessageBox::information(this, tr("Information"), tr("<strong>CodeGenerator v. 4.3</strong><br /><br />Programmeur : Seb2lyon<br />Développé entre le 30-01-2018 et le 05-03-2018<br />GNU General Public License v3.0<br /><br /><a href=http://seb2lyon.site11.com>Visitez mon site web !!!</a>"));
}

// Add attributes in the code
QString *FenPrincipale::ajoutAttributsDansCode(bool etat)
{
    QString *chaine = new QString;

    for(int i = 0; i < nbrAttributs; ++i)
    {
        if(listeAttributs[i].privateAttribut == etat)
        {
            chaine->append(QString("        "));
            if(listeAttributs[i].type == "string" || listeAttributs[i].vector == true)
            {
                chaine->append(QString("std::"));
            }
            if(listeAttributs[i].vector == true)
            {
                chaine->append(QString("vector<"));
                if(listeAttributs[i].type == "string")
                {
                    chaine->append(QString("std::"));
                }
            }
            chaine->append(QString(listeAttributs[i].type));
            if(listeAttributs[i].vector == true)
            {
                chaine->append(QString(">"));
            }
            chaine->append(QString(" "));
            if(listeAttributs[i].pointeur == true)
            {
                chaine->append(QString("*"));
            }
            chaine->append(QString(listeAttributs[i].nom));
            chaine->append(QString(";\n"));
        }
    }

    return chaine;
}

// Getter
QLineEdit *FenPrincipale::getNomClasse()
{
    return nomClasse;
}

QString FenPrincipale::getCheminFichier()
{
    return cheminFichier;
}

QCheckBox *FenPrincipale::getAjoutIncludes()
{
    return ajoutIncludes;
}

QCheckBox *FenPrincipale::getAjoutAttributs()
{
    return ajoutAttributs;
}

QCheckBox *FenPrincipale::getGenereAccesseurs()
{
    return genereAccesseurs;
}

vector<Attribut> FenPrincipale::getListeAttributs()
{
    return listeAttributs;
}

// Setter
void FenPrincipale::setIncludesDispo(vector<QString> nouvelleListeIncludesDispo, int copieNbrIncludesDispo)
{
    nbrIncludesDispo = copieNbrIncludesDispo;

    listeIncludesDispo.clear();
    listeIncludesDispo = nouvelleListeIncludesDispo;
}

void FenPrincipale::setIncludesActifs(vector<QString> nouvelleListeIncludesActifs, int copieNbrIncludesActifs)
{
    nbrIncludesActifs = copieNbrIncludesActifs;

    listeIncludesActifs.clear();
    listeIncludesActifs = nouvelleListeIncludesActifs;
}

void FenPrincipale::setAttributs(vector<Attribut> nouvelleListeAttributs, int copieNbrAttributs)
{
    nbrAttributs = copieNbrAttributs;

    listeAttributs.clear();
    listeAttributs = nouvelleListeAttributs;
}

void FenPrincipale::setCheminFichier(QString nouveauCheminFichier)
{
    cheminFichier = nouveauCheminFichier;
}
