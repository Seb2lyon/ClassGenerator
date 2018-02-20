#include "fencodegenere.h"

using namespace std;

FenCodeGenere::FenCodeGenere(FenPrincipale *fenetre, QString *chaineCodeH, QString *chaineCodeCPP) : QDialog(fenetre, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{    
    setFixedSize(450, 550);

    // Copies of the 2 generated codes
    copieFenPrincipale = fenetre;
    enregistreCodeH = chaineCodeH;
    enregistreCodeCPP = chaineCodeCPP;

    QTabWidget *pageOnglets = new QTabWidget;

    QTextEdit *codeGenereH = new QTextEdit;
    codeGenereH->setPlainText(*chaineCodeH);
    codeGenereH->setReadOnly(true);
    codeGenereH->setFont(QFont("Courier", 9));
    codeGenereH->setLineWrapMode(QTextEdit::NoWrap);

    QTextEdit *codeGenereCPP = new QTextEdit;
    codeGenereCPP->setPlainText(*chaineCodeCPP);
    codeGenereCPP->setReadOnly(true);
    codeGenereCPP->setFont(QFont("Courier", 9));
    codeGenereCPP->setLineWrapMode(QTextEdit::NoWrap);

    nomPageH = fenetre->getNomClasse()->text() + ".h";
    nomPageCPP = fenetre->getNomClasse()->text() + ".cpp";

    pageOnglets->addTab(codeGenereH, nomPageH);
    pageOnglets->addTab(codeGenereCPP, nomPageCPP);

    QPushButton *fermer = new QPushButton(tr("Fermer"));
    fermer->setFixedSize(80, 25);
    QPushButton *enregistrer = new QPushButton(tr("Enregistrer"));
    enregistrer->setFixedSize(80, 25);

    QHBoxLayout *boutons = new QHBoxLayout;
    boutons->addWidget(enregistrer);
    boutons->addWidget(fermer);
    boutons->setAlignment(Qt::AlignRight);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(pageOnglets);
    layout->addLayout(boutons);

    setLayout(layout);

    QObject::connect(fermer, SIGNAL(clicked()), this, SLOT(accept()));
    QObject::connect(enregistrer, SIGNAL(clicked()), this, SLOT(enregistrerCodes()));
}

void FenCodeGenere::enregistrerCodes()
{
    QString cheminFichierH;
    QString cheminFichierCPP;

    if(copieFenPrincipale->getCheminFichier().isEmpty())
    {
        cheminFichierH = nomPageH;
    }
    else
    {
        cheminFichierH = copieFenPrincipale->getCheminFichier() + "/" + nomPageH;
    }

    cheminFichierH = QFileDialog::getSaveFileName(this, tr("Enregistrer le fichier header"), cheminFichierH, tr("Header (*.h)"));
    QString cheminFichier;
    cheminFichier = cheminFichierH.section("/", 0, -2);
    copieFenPrincipale->setCheminFichier(cheminFichier);
    cheminFichierCPP = QFileDialog::getSaveFileName(this, tr("Enregistrer le fichier source"), cheminFichier + "/" + nomPageCPP, tr("Code source (*.cpp)"));

    QFile fichierH(cheminFichierH);
    QFile fichierCPP(cheminFichierCPP);

    fichierH.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream streamFichierH(&fichierH);
    streamFichierH << *enregistreCodeH;

    fichierCPP.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream streamFichierCPP(&fichierCPP);
    streamFichierCPP << *enregistreCodeCPP;
}
