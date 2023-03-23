## Process this file with automake to produce Makefile.in
AUTOMAKE_OPTIONS = foreign
DIST_SUBDIRS = data

AM_CPPFLAGS = -I/usr/include/SDL2 -D_REENTRANT -I ./libini/include 
AM_CXXFLAGS =\
	-Wunused\
	-DDATA_PREFIX=\"$(DATA_PREFIX)\"

CXXFLAGS = -g -O0 -DDEBUG  #debug flags

SUBDIRS = libini

# Note: specifying output into a different directory
bin_PROGRAMS = solitario

# LDFLAGS are not needed at the moment
#solitario_LDFLAGS = -lSDL2

solitario_LDADD =\
	-lSDL2_image\
	-lSDL2_mixer \
	-lSDL2_ttf \
	-lSDL2 \
	libini/libini.a

solitario_SOURCES = \
	src/GameGfx/CardRegionGfx.cpp\
	src/GameGfx/CardStackGfx.cpp\
	src/GameGfx/SolitarioGfx.cpp\
	src/GameGfx/CurrentTime.cpp\
	src/App/MusicManager.cpp\
	src/App/AppGfx.cpp\
	src/App/CardGfx.cpp\
	src/App/DeckType.cpp\
	src/App/GameSettings.cpp\
	src/App/GfxUtil.cpp\
	src/App/Languages.cpp\
	src/App/Main.cpp\
	src/App/Fading.c\
	src/App/Credits.cpp\
	src/App/ErrorInfo.cpp\
	src/App/MenuMgr.cpp\
	src/App/OptionsGfx.cpp\
	src/App/HighScore.cpp\
	src/App/WhereAmI.c\
	src/CompGfx/LabelLinkGfx.cpp\
	src/CompGfx/ButtonGfx.cpp\
	src/CompGfx/CheckBoxGfx.cpp\
	src/CompGfx/ComboGfx.cpp\
	src/CompGfx/MesgBoxGfx.cpp\
	src/CompGfx/LabelGfx.cpp
	