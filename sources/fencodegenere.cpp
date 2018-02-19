#include "fencodegenere.h"

using namespace std;

FenCodeGenere::FenCodeGenere(FenPrincipale *fenetre, QString *chaineCodeH, QString *chaineCodeCPP) : QDialog(fenetre, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{    
    setFixedSize(450, 550);

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

    QString nomPage1 = fenetre->getNomClasse()->text() + ".h";
    QString nomPage2 = fenetre->getNomClasse()->text() + ".cpp";

    pageOnglets->addTab(codeGenereH, nomPage1);
    pageOnglets->addTab(codeGenereCPP, nomPage2);

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
    // Add slot for recording the files
}
