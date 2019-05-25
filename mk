#!/bin/sh
#
# Sample script to run make without having to retype the long path each time
# This will work if you built the environment using our ~/bin/build-snap script

case $1 in
"-l")
	make -C ../../../BUILD/contrib/libutf8 2>&1 | less -SR
	;;

"-d")
	rm -rf ../../../BUILD/contrib/libutf8/doc/libutf8-doc-1.0.tar.gz
	make -C ../../../BUILD/contrib/libutf8
	;;

"-t")
	(
		if make -C ../../../BUILD/contrib/libutf8
		then
			../../../BUILD/contrib/libutf8/tests/unittest --progress
		fi
	) 2>&1 | less -SR
	;;

*)
	make -C ../../../BUILD/contrib/libutf8
	;;

esac
