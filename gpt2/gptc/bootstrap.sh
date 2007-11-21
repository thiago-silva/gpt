#!/bin/sh

aclocal
autoheader
libtoolize -c -f
automake -ac
autoconf
