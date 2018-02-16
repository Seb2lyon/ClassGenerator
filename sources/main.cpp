/***********************************
 *      ClassGenerator v. 4.0      *
 *             -------             *
 *        Coder : Seb2lyon         *
 *  from 01-30-2018 to 16-02-2018  *
 *                                 *
 *             Enjoy ;)            *
 ***********************************/

#include "fenprincipale.h"

int main(int argc, char *argv[])
{
    QApplication appli(argc, argv);
    appli.setWindowIcon(QIcon(QPixmap("images/icone.png")));
    appli.setApplicationName("ClassGenerator v. 4.0");

    QString local = QLocale::system().name().section('_', 0, 0);

    QTranslator translator;
    translator.load(QString("config/ClassGenerator_" + local));

    appli.installTranslator(&translator);

    FenPrincipale fenetre;

    fenetre.show();

    return appli.exec();
}
