#!/bin/sh -e
#
# Verify that the show-utf8 command line tool returns the correct codes

SHOW_UTF8="../../BUILD/Debug/contrib/libutf8/tools/show-utf8"
ERRCNT=0
RED='\033[0;31m'
NORMAL='\033[0m'

# Help
echo "--- SECTION: --help"
if ${SHOW_UTF8} --help
then
	echo "${RED}error: --help returned with success.${NORMAL}"
	ERRCNT=`expr ${ERRCNT} + 1`
elif test ${?} -ne 2
then
	echo "${RED}error: --help did not return with expected exit code.${NORMAL}"
	ERRCNT=`expr ${ERRCNT} + 1`
else
	echo "info: --help works."
fi
echo

# Version
echo "--- SECTION: --version"
if ${SHOW_UTF8} --version
then
	echo "${RED}error: --version returned with success.${NORMAL}"
	ERRCNT=`expr ${ERRCNT} + 1`
elif test ${?} -ne 2
then
	echo "${RED}error: --version did not return with expected exit code.${NORMAL}"
	ERRCNT=`expr ${ERRCNT} + 1`
else
	echo "info: --version works."
fi
echo

# String / Character
echo "--- SECTION: --string"
if ${SHOW_UTF8} "Magic"
then
	echo "info: string display worked."
else
	echo "${RED}error: string display failed with ${?}.${NORMAL}"
	ERRCNT=`expr ${ERRCNT} + 1`
fi
echo

if ${SHOW_UTF8} --string "Élémentaire ça!"
then
	echo "info: string display worked."
else
	echo "${RED}error: string display failed with ${?}.${NORMAL}"
	ERRCNT=`expr ${ERRCNT} + 1`
fi
echo

echo "--- SECTION: --character"
if ${SHOW_UTF8} -C 0x1D11E
then
	echo "info: character display worked."
else
	echo "${RED}error: character display failed with ${?}.${NORMAL}"
	ERRCNT=`expr ${ERRCNT} + 1`
fi
echo

if ${SHOW_UTF8} -C 1D11E
then
	echo "${RED}error: character display succeeded with invalid number syntax.${NORMAL}"
	ERRCNT=`expr ${ERRCNT} + 1`
else
	ERRCODE=${?}
	if test ${ERRCODE} -eq 1
	then
		echo "info: character display failed as expected with ${ERRCODE}."
	else
		echo "${RED}error: character display failed with unexpected error code ${ERRCODE}.${NORMAL}"
		ERRCNT=`expr ${ERRCNT} + 1`
	fi
fi
echo

# Files
check_show() {
	echo "--- SECTION: file with: ${1}"
	if ${SHOW_UTF8} "${1}" tests/example-for-show-${2}.txt
	then
		echo "info: ${2} display worked."
	else
		echo "${RED}error: ${2} display failed with ${?}.${NORMAL}"
		ERRCNT=`expr ${ERRCNT} + 1`
	fi
	echo
}

check_show -f utf8
check_show -S utf16
check_show -F utf32

if test ${ERRCNT} -eq 0
then
	exit 0
fi

echo "${ERRCNT} errors occurred. Please verify what went wrong and fix it."
exit 1

