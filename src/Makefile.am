AM_CPPFLAGS = -I/usr/include/SDL2 -D_REENTRANT -I ./libini/include

AM_CXXFLAGS =\
	-Wunused\
	-DDATA_PREFIX=\"$(DATA_PREFIX)\"

SUBDIRS = libini
bin_PROGRAMS = solitario

solitario_LDFLAGS = -lSDL2

solitario_LDADD =\
	-lSDL2_image\
	-lSDL2_mixer

solitario_SOURCES = \
	CCardRegion.cpp\
	CCardStack.cpp\
	CGame.cpp\
	cHightScoreMgr.cpp\
	cMusicManager.cpp\
	CustomMenu.cpp\
	EngineApp.cpp\
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
	cHightScoreMgr.h\
	cMusicManager.h\
	CustomMenu.h\
	EngineApp.h\
	fonts.h\
	GameSettings.h\
	gfx_util.h\
	Languages.h\
	resource.h\
	StdAfx.h\
	fading.h\
	credits.h\
	win_type_global.h