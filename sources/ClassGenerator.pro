SOURCES += \
    main.cpp \
    fenprincipale.cpp \
    fencodegenere.cpp \
    fenincludes.cpp \
    fenattributs.cpp

HEADERS += \
    fenprincipale.h \
    fencodegenere.h \
    fenincludes.h \
    fenattributs.h

OTHER_FILES += \
    ressources.rc

RC_FILE = ressources.rc

TRANSLATIONS = ClassGenerator_en.ts ClassGenerator_pl.ts

QT += widgets
