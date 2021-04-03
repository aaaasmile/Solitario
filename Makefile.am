## Process this file with automake to produce Makefile.in

## Created by Anjuta - will be overwritten
## If you don't want it to overwrite it,
## 	Please disable it in the Anjuta project configuration

DIST_SUBDIRS = data
SUBDIRS = data

INCLUDES =\
	-I/usr/include/SDL

AM_CXXFLAGS =\
	-Wunused\
	-DDATA_PREFIX=\"$(DATA_PREFIX)\"
	
#DATA_PREFIX=/usr/share/solitario/
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
	win_type_global.h

solitario_LDFLAGS =\
	$(shell sdl-config --libs) 

solitario_LDADD =\
	-lSDL_image\
	-lSDL_mixer\
	-lini

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
