all: self
	@echo   "[boostraping common...]" 
	cd common && $(MAKE) -f Makefile.boot
	@echo   "[boostraping gptasm...]" 
	cd gptasm && $(MAKE) -f Makefile.boot
	@echo   "[boostraping gptc...]" 
	cd gptc && $(MAKE) -f Makefile.boot
	@echo   "[boostraping gptvm...]" 
	cd gptvm && $(MAKE) -f Makefile.boot

self:
	@echo   "[boostraping self...]" 
	aclocal
	autoheader
	libtoolize -c -f
	automake -ac
	autoconf

distclean: clean-self
	cd common && $(MAKE) -f Makefile.boot distclean
	cd gptc && $(MAKE) -f Makefile.boot distclean
	cd gptvm && $(MAKE) -f Makefile.boot distclean
	cd gptasm && $(MAKE) -f Makefile.boot distclean

clean-self:
	rm -rf autom4te.cache aclocal.m4 config.* configure depcomp install-sh missing ltmain.sh
