QT += core widgets openglwidgets

TARGET = Knots
TEMPLATE = app
CONFIG += console
CONFIG += c++1z
win32 {
    LIBS += -lopengl32
    LIBS += -lglu32
}
CONFIG += warn_on
CONFIG += debug

INCLUDEPATH += include src

#include(src/src.pri)

FORMS += forms/mainwindow.ui

RESOURCES += glsl.qrc \
    txt.qrc

*-clang*|*-g++* {
    message("Enabling additional warnings")
    CONFIG -= warn_on
    QMAKE_CXXFLAGS += -Wall -Wextra -pedantic -Winit-self
    QMAKE_CXXFLAGS += -Wno-strict-aliasing
    QMAKE_CXXFLAGS += -fno-omit-frame-pointer
}
linux-clang*|linux-g++*|macx-clang*|macx-g++* {
    message("Enabling stack protector")
    QMAKE_CXXFLAGS += -fstack-protector-all
}

# FOR LINUX & MAC USERS INTERESTED IN ADDITIONAL BUILD TOOLS
# ----------------------------------------------------------
# This conditional exists to enable Address Sanitizer (ASAN) during
# the automated build. ASAN is a compiled-in tool which checks for
# memory errors (like Valgrind). You may enable it for yourself;
# check the hidden `.build.sh` file for info. But be aware: ASAN may
# trigger a lot of false-positive leak warnings for the Qt libraries.
# (See `.run.sh` for how to disable leak checking.)
address_sanitizer {
    message("Enabling Address Sanitizer")
    QMAKE_CXXFLAGS += -fsanitize=address
    QMAKE_LFLAGS += -fsanitize=address
}

HEADERS += \
    src/BSpline.h \
    src/Camera.h \
    src/Curve.h \
    src/FileReader.h \
    src/Node.h \
    src/Rope.h \
    src/Vector.h \
    src/cameracontrols.h \
    src/drawable.h \
    src/glwidget277.h \
    src/la.h \
    src/mainwindow.h \
    src/mygl.h \
    src/openglcontext.h \
    src/scene/Cube.h \
    src/scene/Cylinder.h \
    src/scene/Geometry.h \
    src/scene/Mesh.h \
    src/scene/Sphere.h \
    src/scene/Voxel.h \
    src/scene/line.h \
    src/shaderprogram.h \
    src/smartpointerhelp.h \
    src/utils.h

SOURCES += \
    src/BSpline.cpp \
    src/Camera.cpp \
    src/Curve.cpp \
    src/FileReader.cpp \
    src/Node.cpp \
    src/Rope.cpp \
    src/Vector.cpp \
    src/cameracontrols.cpp \
    src/drawable.cpp \
    src/glwidget277.cpp \
    src/la.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mygl.cpp \
    src/openglcontext.cpp \
    src/scene/Cube.cpp \
    src/scene/Cylinder.cpp \
    src/scene/Geometry.cpp \
    src/scene/Mesh.cpp \
    src/scene/Sphere.cpp \
    src/scene/Voxel.cpp \
    src/scene/line.cpp \
    src/shaderprogram.cpp \
    src/utils.cpp
