# project.pro - Файл проекта для qmake
QT += core gui widgets webengine webenginewidgets concurrent

CONFIG += c++17

# Название приложения
TARGET = VueQtApp

# Задаем директорию для исполняемого файла
DESTDIR = $$PWD/bin

# Директория для объектных файлов и промежуточных файлов сборки
OBJECTS_DIR = $$PWD/bin/dep/obj
MOC_DIR = $$PWD/bin/dep/moc
RCC_DIR = $$PWD/bin/dep/rcc
UI_DIR = $$PWD/bin/dep/ui

SOURCES += \
    cpp-server/src/main.cpp \
    cpp-server/src/backend.cpp \
    cpp-server/src/drawings_handler.cpp

HEADERS += \
    cpp-server/src/backend.h \
    cpp-server/src/drawings_handler.h

RESOURCES += \
    resources.qrc

win32 {
    QMAKE_POST_LINK += mkdir -p $$shell_path($$DESTDIR/frontend/dist) && \
                      xcopy /E /I /Y $$shell_path($$PWD/vue-client/dist) $$shell_path($$DESTDIR/frontend/dist)
} else {
    QMAKE_POST_LINK += mkdir -p $$shell_path($$DESTDIR/frontend/dist) && \
                      cp -r $$shell_path($$PWD/vue-client/dist) $$shell_path($$DESTDIR/frontend)
}