TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CC= gcc -std=c99

SOURCES += \
    tmf8x2x_spad_mask_tool.c \
    tmf8x2x_test_masks.c

HEADERS += \
    tmf8x2x_includes.h \
    tmf8x2x_spad_mask_tool.h

CONFIG += outputInWorkspace

DISTFILES += \
    readme.md
