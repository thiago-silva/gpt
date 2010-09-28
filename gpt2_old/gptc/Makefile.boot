all:
	aclocal
	autoheader
	libtoolize -c -f
	automake -ac
	autoconf

distclean:
	rm -rf autom4te.cache aclocal.m4 config.* configure depcomp install-sh missing ltmain.sh
