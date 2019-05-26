# - Try to find LibUtf8
#
# Once done this will define
#
# LIBUTF8_FOUND        - System has LibUtf8
# LIBUTF8_INCLUDE_DIR  - The LibUtf8 include directories
# LIBUTF8_LIBRARY      - The libraries needed to use LibUtf8 (none)
# LIBUTF8_DEFINITIONS  - Compiler switches required for using LibUtf8 (none)

find_path( LIBUTF8_INCLUDE_DIR libutf8/libutf8.h
		   PATHS $ENV{LIBUTF8_INCLUDE_DIR}
		 )
find_library( LIBUTF8_LIBRARY utf8
		   PATHS $ENV{LIBUTF8_LIBRARY}
		 )
mark_as_advanced( LIBUTF8_INCLUDE_DIR LIBUTF8_LIBRARY )

set( LIBUTF8_INCLUDE_DIRS ${LIBUTF8_INCLUDE_DIR} )
set( LIBUTF8_LIBRARIES    ${LIBUTF8_LIBRARY}     )

include( FindPackageHandleStandardArgs )
# handle the QUIETLY and REQUIRED arguments and set LIBUTF8_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args( LibUtf8 DEFAULT_MSG LIBUTF8_INCLUDE_DIR LIBUTF8_LIBRARY )
