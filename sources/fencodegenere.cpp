#include "fencodegenere.h"

using namespace std;

FenCodeGenere::FenCodeGenere(FenPrincipale *fenetre, QString *chaineCode) : QDialog(fenetre)
{    
    setFixedSize(400, 550);

    QTextEdit *codeGenere = new QTextEdit;
    codeGenere->setText(*chaineCode);
    codeGenere->setReadOnly(true);
    codeGenere->setFont(QFont("Courier", 9));

    QPushButton *fermer = new QPushButton(tr("Fermer"));
    fermer->setFixedHeight(25);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(codeGenere);
    layout->addWidget(fermer);

    setLayout(layout);

    QObject::connect(fermer, SIGNAL(clicked()), this, SLOT(accept()));
}
