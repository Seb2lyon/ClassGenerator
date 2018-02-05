/***********************************
 *      ClassGenerator v. 3.0      *
 *             -------             *
 *        Coder : Seb2lyon         *
 *  from 01-30-2018 to 05-02-2018  *
 *                                 *
 *             Enjoy ;)            *
 ***********************************/

#include "fenprincipale.h"

int main(int argc, char *argv[])
{
    QApplication appli(argc, argv);
    appli.setWindowIcon(QIcon(QPixmap("icone.png")));
    appli.setApplicationName("ClassGenerator v. 3.0");

    QString local = QLocale::system().name().section('_', 0, 0);

    QTranslator translator;
    translator.load(QString("ClassGenerator_" + local));

    appli.installTranslator(&translator);

    FenPrincipale fenetre;

    fenetre.show();

    return appli.exec();
}
