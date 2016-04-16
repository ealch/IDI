TEMPLATE    = app
QT         += opengl

LIBS += -lGLEW
INCLUDEPATH +=  /usr/include/glm \
		./Model

HEADERS += MyGLWidget.h

SOURCES += main.cpp \
        MyGLWidget.cpp \
        Model/model.cpp
