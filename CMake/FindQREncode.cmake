find_path(QRENCODE_INCLUDE_DIR qrencode.h
	HINTS
	$ENV{QRENCODE_DIR}
	PATH_SUFFIXES include/qrencode include
	PATHS
	/usr/local
	/usr
)

find_library(QRENCODE_LIBRARY
	NAMES qrencode
	HINTS
	$ENV{QRENCODE_DIR}
)

if(QRENCODE_LIBRARY AND QRENCODE_INCLUDE_DIR)
	message(STATUS "Found QREncode: ${QRENCODE_LIBRARY}")
	set(QRENCODE_FOUND TRUE)
endif()
