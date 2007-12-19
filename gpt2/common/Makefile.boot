all:
	touch NEWS README AUTHORS ChangeLog
	aclocal
	autoheader
	libtoolize -c -f
	automake -ac
	autoconf
	rm -rf NEWS README AUTHORS ChangeLog COPYING INSTALL

distclean:
	rm -rf autom4te.cache AUTHORS COPYING ChangeLog INSTALL NEWS README \
         aclocal.m4 config.* configure depcomp install-sh missing ltmain.sh
