## Process this file with automake to produce Makefile.in

SUBDIRS = src data

DIST_SUBDIRS = data
	
DATA_PREFIX=data/

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
