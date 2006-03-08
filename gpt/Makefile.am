# not a GNU package. You can remove this line, if
# have all needed files, that a GNU package needs
AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = src doc exemplos

EXTRA_DIST = INSTALL.default ChangeLog

uninstall-devel:
	rm -rf $(includedir)/${PACKAGE} 
	rm -f $(libdir)/libgportugol*