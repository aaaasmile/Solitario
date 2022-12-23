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
	-lSDL2 \
	libini/libini.a

solitario_SOURCES = \
	src/CCardRegion.cpp\
	src/CCardStack.cpp\
	src/SolitarioGfx.cpp\
	src/HightScoreMgr.cpp\
	src/MusicManager.cpp\
	src/CustomMenu.cpp\
	src/AppGfx.cpp\
	src/fonts.cpp\
	src/GameSettings.cpp\
	src/gfx_util.cpp\
	src/Languages.cpp\
	src/main.cpp\
	src/fading.cpp\
	src/credits.cpp\
	src/error_info.cpp
	