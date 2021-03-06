#!/bin/bash

test -n "$srcdir" || srcdir=`dirname "$0"`
test -n "$srcdir" || srcdir=.

olddir=`pwd`
cd "$srcdir"

aclocal
if test $? != 0 ; then
	echo "aclocal failed."
	exit -1
fi

autoconf
if test $? != 0 ; then
	echo "autoconf failed."
	exit -1
fi

automake --add-missing 2> /dev/null | true

cd "$olddir"
test -n "$NOCONFIGURE" || "$srcdir/configure" "$@"




