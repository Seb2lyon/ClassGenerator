#include "fenprincipale.h"
#include "fencodegenere.h"

FenPrincipale::FenPrincipale():QWidget()
{
    setFixedSize(450, 500);

    QValidator *validator = new QRegularExpressionValidator(QRegularExpression("[_A-Za-z0-9]{0,32}"));

    // 1) Form "Class definition"
    this->nomClasse = new QLineEdit;
    nomClasse->setStyleSheet("background: rgb(255,255,255)");
    nomClasse->setValidator(validator);
    this->nomClasseMere = new QLineEdit;
    nomClasseMere->setStyleSheet("background: rgb(255,255,255)");
    nomClasseMere->setValidator(validator);

    // Form Layout
    QFormLayout *defClasseLayout = new QFormLayout;
    defClasseLayout->addRow("&Nom : ", nomClasse);
    defClasseLayout->addRow("Classe &mère : ", nomClasseMere);

    // Set the form layout into QGroupBox
    QGroupBox *defClasse = new QGroupBox("Définition de la classe");
    defClasse->setLayout(defClasseLayout);


    // 2) Checkbox "Options"
    this->protegeHeader = new QCheckBox;
    protegeHeader->setText("Protéger le &header contre les inclusions multiples");
    protegeHeader->setChecked(true);
    this->genereConstructeur = new QCheckBox;
    genereConstructeur->setText("Générer un &constructeur par défaut");
    genereConstructeur->setChecked(true);
    this->genereDestructeur = new QCheckBox;
    genereDestructeur->setText("Générer un &destructeur");

    // Form Layout
    QFormLayout *optionsLayout = new QFormLayout;
    optionsLayout->addRow(protegeHeader);
    optionsLayout->addRow(genereConstructeur);
    optionsLayout->addRow(genereDestructeur);

    // Set the form layout into QGroupBox
    QGroupBox *options = new QGroupBox("Options");
    options->setLayout(optionsLayout);


    // 3) Lignes du formulaire "Ajouter des commentaires"
    this->auteur = new QLineEdit;
    this->dateCreation = new QDateEdit(QDate::currentDate());
    this->roleClasse = new QTextEdit;

    // Form Layout
    QFormLayout *ajoutCommentairesLayout = new QFormLayout;
    ajoutCommentairesLayout->addRow("&Auteur : ", auteur);
    ajoutCommentairesLayout->addRow("Da&te de création : ", dateCreation);
    ajoutCommentairesLayout->addRow("&Rôle de la classe : ", roleClasse);

    // Set the form layout into QGroupBox
    this->ajoutCommentaires = new QGroupBox("Ajouter des commentaires");
    ajoutCommentaires->setCheckable(true);
    ajoutCommentaires->setChecked(false);
    ajoutCommentaires->setLayout(ajoutCommentairesLayout);


    // 4) Create buttons
    QPushButton *generer = new QPushButton("Générez !");
    generer->setFixedSize(80, 25);
    QPushButton *quitter = new QPushButton("Quitter");
    quitter->setFixedSize(80, 25);

    // Buttons layout
    QHBoxLayout *boutons = new QHBoxLayout;
    boutons->addWidget(generer);
    boutons->addWidget(quitter);
    boutons->setAlignment(Qt::AlignRight);


    // Generate global layout
    QVBoxLayout *layoutGlobal = new QVBoxLayout;
    layoutGlobal->addWidget(defClasse);
    layoutGlobal->addWidget(options);
    layoutGlobal->addWidget(ajoutCommentaires);
    layoutGlobal->addLayout(boutons);

    this->setLayout(layoutGlobal);

    QObject::connect(quitter, SIGNAL(clicked()), qApp, SLOT(quit()));
    QObject::connect(generer, SIGNAL(clicked()), this, SLOT(validerFenPrincipale()));
}

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
        QMessageBox::warning(this, "Attention", "Vous devez renseigner le nom de la Classe");
    }
    else if(validator->validate(inputClass, pos) != QValidator::Acceptable)
    {
        nomClasse->setStyleSheet("background: rgb(252,254,171)");
        nomClasseMere->setStyleSheet("background: rgb(255,255,255)");
        nomClasse->setFocus(Qt::OtherFocusReason);
        QMessageBox::warning(this, "Attention", "Le nom de la Classe est invalide :\nLe nom ne doit pas commencer par un chiffre");
    }
    else if(!nomClasseMere->text().isEmpty() && validator->validate(inputClassMere, pos) != QValidator::Acceptable)
    {
        nomClasse->setStyleSheet("background: rgb(255,255,255)");
        nomClasseMere->setStyleSheet("background: rgb(252,254,171)");
        nomClasse->setStyleSheet("background: rgb(255,255,255)");
        nomClasseMere->setFocus(Qt::OtherFocusReason);
        QMessageBox::warning(this, "Attention", "Le nom de la Classe mère est invalide :\nLe nom ne doit pas commencer par un chiffre");
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
                chaine->append(QString("Auteur : "));
                chaine->append(QString(auteur->text()));
                chaine->append(QString("\n"));
            }

            chaine->append(QString("Date de création : "));
            chaine->append(QString(dateCreation->date().toString(Qt::TextDate)));
            chaine->append(QString("\n"));

            // If the role's text is written
            if(!roleClasse->toPlainText().isEmpty())
            {
                chaine->append(QString("\nRôle :\n"));
                chaine->append(QString(roleClasse->toPlainText()));
                chaine->append(QString("\n"));
            }

            chaine->append(QString("*/\n\n\n"));
        }

        // Checkbox checked : Header protecteed
        if(protegeHeader->isChecked())
        {
            chaine->append(QString("#ifndef HEADER_"));
            chaine->append(QString(nomClasse->text().toUpper()));
            chaine->append(QString("\n#define HEADER_"));
            chaine->append(QString(nomClasse->text().toUpper()));
            chaine->append(QString("\n\n\n"));
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
