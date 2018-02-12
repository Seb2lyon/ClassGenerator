#include "fenprincipale.h"
#include "fencodegenere.h"
#include "fenincludes.h"
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
    ajoutIncludes->setText(tr("Aj&outer des includes"));
    ajoutAttributs = new QCheckBox;
    ajoutAttributs->setText(tr("Ajo&uter des attributs"));

    // Form Layout
    QFormLayout *optionsLayout = new QFormLayout;
    optionsLayout->addRow(protegeHeader);
    optionsLayout->addRow(headerGuard);
    optionsLayout->addRow(genereConstructeur);
    optionsLayout->addRow(genereDestructeur);
    optionsLayout->addRow(ajoutIncludes);
    optionsLayout->addRow(ajoutAttributs);

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
    ajoutCommentairesLayout->addRow(tr("&Auteur : "), auteur);
    ajoutCommentairesLayout->addRow(tr("Da&te de création : "), dateCreation);
    ajoutCommentairesLayout->addRow(tr("&Licence : "), license);
    ajoutCommentairesLayout->addRow(tr("&Rôle de la classe : "), roleClasse);

    // Set the form layout into QGroupBox
    ajoutCommentaires = new QGroupBox(tr("Ajouter des commentaires"));
    ajoutCommentaires->setCheckable(true);
    ajoutCommentaires->setChecked(false);
    ajoutCommentaires->setLayout(ajoutCommentairesLayout);


    // 4) Create buttons
    QPushButton *generer = new QPushButton(tr("Générez !"));
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

    std::ifstream fichier("config/includes.conf");

    std::string ligne;
    int occurences = 0;

    while(std::getline(fichier, ligne))
    {
        ligne = "<" + ligne + ">";
        listeIncludesDispo.push_back(QString::fromStdString(ligne));
        ++occurences;
    }

    // Number of includes
    nbrIncludesDispo = occurences;
    nbrIncludesActifs = 0;

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
    else
    {
        nomClasse->setStyleSheet("background: rgb(255,255,255)");
        nomClasseMere->setStyleSheet("background: rgb(255,255,255)");

        QString *chaine = new QString;

        // Coments activated
        if(ajoutCommentaires->isChecked())
        {
            chaine->append(QString("/*\n"));

            // If the author's name is set
            if(!auteur->text().isEmpty())
            {
                chaine->append(QString(tr("Auteur : ")));
                chaine->append(QString(auteur->text()));
                chaine->append(QString("\n"));
            }

            chaine->append(QString(tr("Date de création : ")));
            chaine->append(QString(dateCreation->date().toString(Qt::SystemLocaleLongDate)));
            chaine->append(QString("\n"));

            // If licence is set
            if(license->currentIndex() != 0 && license->currentText() != QString(tr("Autre... (modifiez cette ligne)")) && !license->currentText().isEmpty())
            {
                chaine->append(tr("\nCe logiciel est sous licence : "));
                chaine->append(license->currentText());
                chaine->append("\n");
            }

            // If the role's text is written
            if(!roleClasse->toPlainText().isEmpty())
            {
                chaine->append(QString(tr("\nRôle :\n")));
                chaine->append(QString(roleClasse->toPlainText()));
                chaine->append(QString("\n"));
            }

            chaine->append(QString("*/\n\n\n"));
        }

        // Checkbox checked : Header protected
        if(protegeHeader->isChecked())
        {
            // If the user delete the generated Header Guard
            if(headerGuard->text().isEmpty())
            {
                chaine->append(QString("#ifndef HEADER_"));
                chaine->append(QString(nomClasse->text().toUpper()));
                chaine->append(QString("\n#define HEADER_"));
                chaine->append(QString(nomClasse->text().toUpper()));
            }
            else
            {
                chaine->append(QString("#ifndef "));
                chaine->append(QString(headerGuard->text().toUpper()));
                chaine->append(QString("\n#define "));
                chaine->append(QString(headerGuard->text().toUpper()));
            }

            chaine->append(QString("\n\n\n"));
        }

        if(ajoutIncludes->isChecked() && nbrIncludesActifs != 0)
        {
            for(int i = 0; i < nbrIncludesActifs; i++)
            {
                chaine->append("#include ");
                chaine->append(listeIncludesActifs[i]);
                chaine->append("\n");
            }

            chaine->append("\n\n");
        }

        chaine->append(QString("class "));
        chaine->append(QString(nomClasse->text()));

        // Mother-class set
        if(!nomClasseMere->text().isEmpty())
        {
            chaine->append(QString(" : public "));
            chaine->append(QString(nomClasseMere->text()));
        }

        chaine->append(QString("\n{\n    public:\n"));

        // Checkbox checked : Constructor generated
        if(genereConstructeur->isChecked())
        {
            chaine->append(QString("        "));
            chaine->append(QString(nomClasse->text()));
            chaine->append(QString("();\n"));
        }

        // Checkbox checked : Destructor generated
        if(genereDestructeur->isChecked())
        {
            chaine->append(QString("        ~"));
            chaine->append(QString(nomClasse->text()));
            chaine->append(QString("();\n"));
        }

        chaine->append(QString("\n\n    protected:\n\n\n    private:\n\n\n};\n\n#endif\n\n"));

        FenCodeGenere fenetreCode(this, chaine);

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
    // TODO
}

// Information window
void FenPrincipale::fenetreInfo()
{
   QMessageBox::information(this, tr("Information"), tr("<strong>CodeGenerator v. 3.0</strong><br /><br />Programmeur : Seb2lyon<br />Développé entre le 30-01-2018 et le 05-02-2018<br />GNU General Public License v3.0<br /><br /><a href=http://seb2lyon.site11.com>Visitez mon site web !!!</a>"));
}

// Getter
QCheckBox *FenPrincipale::getAjoutIncludes()
{
    return ajoutIncludes;
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
