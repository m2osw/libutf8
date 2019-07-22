#!/bin/sh
#
# Sample script to run make without having to retype the long path each time
# This will work if you built the environment using our ~/bin/build-snap script

PROCESSORS=4

case $1 in
"-l")
	make -C ../../../BUILD/contrib/libutf8 2>&1 | less -SR
	;;

"-d")
	rm -rf ../../../BUILD/contrib/libutf8/doc/libutf8-doc-1.0.tar.gz
	make -C ../../../BUILD/contrib/libutf8
	;;

"-i")
	make -j${PROCESSORS} -C ../../../BUILD/contrib/libutf8 install
	;;

"-t")
	(
		if make -j${PROCESSORS} -C ../../../BUILD/contrib/libutf8
		then
			shift
			../../../BUILD/contrib/libutf8/tests/unittest --progress $*
		fi
	) 2>&1 | less -SR
	;;

"")
	make -j${PROCESSORS} -C ../../../BUILD/contrib/libutf8
	;;

*)
	echo "error: unknown command line option \"$1\""
	;;

esac
