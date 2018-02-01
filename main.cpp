#include "fenprincipale.h"

int main(int argc, char *argv[])
{
    QApplication appli(argc, argv);
    appli.setWindowIcon(QIcon(QPixmap("icone.png")));

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    appli.installTranslator(&translator);

    FenPrincipale fenetre;

    fenetre.show();

    return appli.exec();
}
