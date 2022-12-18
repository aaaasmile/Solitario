## Process this file with automake to produce Makefile.in

## Created by Anjuta - will be overwritten
## If you don't want it to overwrite it,
## 	Please disable it in the Anjuta project configuration

DIST_SUBDIRS = data
SUBDIRS = data

AM_CPPFLAGS = -I/usr/include/SDL2 -D_REENTRANT -I ./libini/libini-1.1.10/include

AM_CXXFLAGS =\
	-Wunused\
	-DDATA_PREFIX=\"$(DATA_PREFIX)\"
	
DATA_PREFIX=data/

bin_PROGRAMS = solitario

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
	win_type_global.h\
	./libini/libini-1.1.10/src/headings.h\ 
	./libini/libini-1.1.10/src/ini.cpp\
	./libini/libini-1.1.10/src/ini.h\
  ./libini/libini-1.1.10/src/keys.h\
	./libini/libini-1.1.10/src/list.h

solitario_LDFLAGS = -lSDL2

solitario_LDADD =\
	-lSDL2_image\
	-lSDL2_mixer

MAINTAINERCLEANFILES =\
	Makefile.in\
	aclocal.m4\
	configure\
	decomp\
	install-sh\
	missing\
	mkinstalldirs\
	$(PACKAGE)-$(VERSION).tar.gz

maintainer-clean-local:
	-rm -rf $(RPM_TOPDIR)/BUILD
	-rm -rf $(RPM_TOPDIR)/RPMS
	-rm -rf $(RPM_TOPDIR)/SOURCES
	-rm -rf $(RPM_TOPDIR)/SPECS
	-rm -rf $(RPM_TOPDIR)/SRPMS

rpm: dist $(PACKAGE).spec
	$(mkinstalldirs) $(RPM_TOPDIR)/{BUILD,RPMS,SOURCES,SPECS,SRPMS}
	$(INSTALL_DATA) $(PACKAGE).spec $(RPM_TOPDIR)/SPECS
	$(INSTALL_DATA) $(PACKAGE)-$(VERSION).tar.gz $(RPM_TOPDIR)/SOURCES
	rpmbuild -ba $(PACKAGE).spec\
	--define "_topdir $(RPM_TOPDIR)"\
	--define "_prefix $(prefix)"\
	--define "_datadir $(prefix)/share"\
	--define "_defaultdocdir $(prefix)/share/doc"\
	--clean

.PHONY: rpm
