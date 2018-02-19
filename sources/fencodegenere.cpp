#include "fencodegenere.h"

using namespace std;

FenCodeGenere::FenCodeGenere(FenPrincipale *fenetre, QString *chaineCodeH, QString *chaineCodeCPP) : QDialog(fenetre, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{    
    setFixedSize(450, 550);

    QTextEdit *codeGenere = new QTextEdit;
    codeGenere->setPlainText(*chaineCodeH);
    codeGenere->setReadOnly(true);
    codeGenere->setFont(QFont("Courier", 9));
    codeGenere->setLineWrapMode(QTextEdit::NoWrap);

    QPushButton *fermer = new QPushButton(tr("Fermer"));
    fermer->setFixedHeight(25);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(codeGenere);
    layout->addWidget(fermer);

    setLayout(layout);

    QObject::connect(fermer, SIGNAL(clicked()), this, SLOT(accept()));
}
