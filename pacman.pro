TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    bomg.cpp \
    game_types.cpp \
    ghost.cpp \
    ghost_manager.cpp \
    graphics.cpp \
    pacman.cpp \
    score.cpp \
    tools.cpp

message($$PWD)
SFML_PATH= $$PWD/../3rdparty/SFML-2.6.0
message($$SFML_PATH)
INCLUDEPATH += $$SFML_PATH/include
LIBS += -L$$SFML_PATH/lib -lFLAC \
                         -lfreetype \
                         -logg \
                         -lopenal32 \
                         -lsfml-audio-d \
                         -lsfml-audio-s-d \
                         -lsfml-audio-s \
                         -lsfml-audio \
                         -lsfml-graphics-d \
                         -lsfml-graphics-s-d \
                         -lsfml-graphics-s \
                         -lsfml-graphics \
                         -lsfml-main-d \
                         -lsfml-main \
                         -lsfml-network-d \
                         -lsfml-network-s-d \
                         -lsfml-network-s \
                         -lsfml-network \
                         -lsfml-system-d \
                         -lsfml-system-s-d \
                         -lsfml-system-s \
                         -lsfml-system \
                         -lsfml-window-d \
                         -lsfml-window-s-d \
                         -lsfml-window-s \
                         -lsfml-window \
                         -lvorbis \
                         -lvorbisenc \
                         -lvorbisfile

HEADERS += \
    bomg.h \
    game_types.h \
    ghost.h \
    ghost_manager.h \
    graphics.h \
    pacman.h \
    score.h \
    tools.h \
    constants.h
