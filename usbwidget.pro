#-------------------------------------------------
#
# Project created by QtCreator 2014-05-13T00:57:10
#
#-------------------------------------------------

QT       += core gui \
	network \
	script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = usbwidget
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    gengxin.cpp \
    wenjian.cpp \
    baobiao.cpp \
    fuwuqi.cpp \
    mainwindow.cpp \
    shangchuan.cpp \
    global.cpp \
    devwdg.cpp \
    devprocess.cpp \
    help.cpp \
    dataupdate.cpp \
    fileupload.cpp \
    usb_state.cpp \
    sqlopt.cpp \
    record.cpp \
    data_sql.cpp \
    upstate.cpp \
    threadcontrol.cpp

HEADERS  += widget.h \
    gengxin.h \
    ClickedLabel.h \
    wenjian.h \
    baobiao.h \
    fuwuqi.h \
    mainwindow.h \
    shangchuan.h \
    global.h \
    devwdg.h \
    devprocess.h \
    help.h \
    dataupdate.h \
    fileupload.h \
    parser.h \
    qjson_export.h \
    usb_enum.h \
    sqlopt.h \
    record.h \
    data_sql.h \
    upstate.h \
    threadcontrol.h


FORMS    += widget.ui \
    gengxin.ui \
    wenjian.ui \
    baobiao.ui \
    fuwuqi.ui \
    mainwindow.ui \
    shangchuan.ui \
    devwdg.ui \
    help.ui \
    upstate.ui

RESOURCES += \
    usbwidget.qrc

LIBS += -lsqlite3 \
            /usr/local/lib/libusb_enum.a


