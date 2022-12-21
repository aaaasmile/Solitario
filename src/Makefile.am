AM_CPPFLAGS = -I/usr/include/SDL2 -D_REENTRANT -I ./libini/include

AM_CXXFLAGS =\
	-Wunused\
	-DDATA_PREFIX=\"$(DATA_PREFIX)\"

SUBDIRS = libini
bin_PROGRAMS = solitario

solitario_LDFLAGS = -lSDL2

solitario_LDADD =\
	-lSDL2_image\
	-lSDL2_mixer \
	-lSDL2 \
	libini/libini.a

solitario_SOURCES = \
	CCardRegion.cpp\
	CCardStack.cpp\
	CGame.cpp\
	HightScoreMgr.cpp\
	MusicManager.cpp\
	CustomMenu.cpp\
	AppGfx.cpp\
	fonts.cpp\
	GameSettings.cpp\
	gfx_util.cpp\
	Languages.cpp\
	main.cpp\
	fading.cpp\
	credits.cpp\
	CCard.h\
	CCardRegion.h\
	CCardStack.h\
	CGame.h\
	HightScoreMgr.h\
	MusicManager.h\
	CustomMenu.h\
	AppGfx.h\
	fonts.h\
	GameSettings.h\
	gfx_util.h\
	Languages.h\
	resource.h\
	StdAfx.h\
	fading.h\
	credits.h\
	win_type_global.h