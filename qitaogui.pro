QT += widgets
QT += core 
QT += sql
QT += serialport

HEADERS       = wigglywidget.h \
                dialog.h \
    qitaojiemian.h \
    chukujiemian.h \
    qwcomboboxdelegate.h \
    ruku.h

SOURCES       = wigglywidget.cpp \
                dialog.cpp \
                main.cpp \
    qitaojiemian.cpp \
    chukujiemian.cpp \
    qwcomboboxdelegate.cpp \
    ruku.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/wiggly
INSTALLS += target

DISTFILES +=

RESOURCES += \
    res.qrc

FORMS += \
    qitaojiemian.ui \
    chukujiemian.ui \
    ruku.ui
