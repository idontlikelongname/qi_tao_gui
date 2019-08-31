QT += widgets
QT += core 
QT += sql

HEADERS       = wigglywidget.h \
                dialog.h \
    qitaojiemian.h \
    chukujiemian.h \
    qwcomboboxdelegate.h

SOURCES       = wigglywidget.cpp \
                dialog.cpp \
                main.cpp \
    qitaojiemian.cpp \
    chukujiemian.cpp \
    qwcomboboxdelegate.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/wiggly
INSTALLS += target

DISTFILES +=

RESOURCES += \
    res.qrc

FORMS += \
    qitaojiemian.ui \
    chukujiemian.ui
