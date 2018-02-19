#include "fencodegenere.h"

using namespace std;

FenCodeGenere::FenCodeGenere(FenPrincipale *fenetre, QString *chaineCodeH, QString *chaineCodeCPP) : QDialog(fenetre, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{    
    setFixedSize(450, 550);

    QTabWidget *pageOnglets = new QTabWidget;
    QWidget *page1 = new QWidget;
    QWidget *page2 = new QWidget;

    QTextEdit *codeGenereH = new QTextEdit;
    codeGenereH->setPlainText(*chaineCodeH);
    codeGenereH->setReadOnly(true);
    codeGenereH->setFont(QFont("Courier", 9));
    codeGenereH->setLineWrapMode(QTextEdit::NoWrap);
    QVBoxLayout *layoutH = new QVBoxLayout;
    layoutH->addWidget(codeGenereH);

    page1->setLayout(layoutH);

    QTextEdit *codeGenereCPP = new QTextEdit;
    codeGenereCPP->setPlainText(*chaineCodeCPP);
    codeGenereCPP->setReadOnly(true);
    codeGenereCPP->setFont(QFont("Courier", 9));
    codeGenereCPP->setLineWrapMode(QTextEdit::NoWrap);
    QVBoxLayout *layoutCPP = new QVBoxLayout;
    layoutCPP->addWidget(codeGenereCPP);

    page2->setLayout(layoutCPP);

    QString nomPage1 = fenetre->getNomClasse()->text() + ".h";
    QString nomPage2 = fenetre->getNomClasse()->text() + ".cpp";

    pageOnglets->addTab(page1, nomPage1);
    pageOnglets->addTab(page2, nomPage2);

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
