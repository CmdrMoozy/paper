find_path(LIBLZMA_INCLUDE_DIR lzma.h
	HINTS
	$ENV{LIBLZMA_DIR}
	PATH_SUFFIXES include/lzma include
	PATHS
	/usr/local
	/usr
)

find_library(LIBLZMA_LIBRARY
	NAMES lzma
	HINTS
	$ENV{LIBLZMA_DIR}
)

if(LIBLZMA_LIBRARY AND LIBLZMA_INCLUDE_DIR)
	message(STATUS "Found liblzma: ${LIBLZMA_LIBRARY}")
	set(LIBLZMA_FOUND TRUE)
endif()
