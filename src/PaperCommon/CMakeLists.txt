set(PaperCommon_SOURCES

	Functionality.cpp
	Functionality.h
	QRCode.cpp
	QRCode.h

	Compression/LZMA.cpp
	Compression/LZMA.h

	Util/IO.cpp
	Util/IO.h
	Util/Memory.h
	Util/Memstream.cpp
	Util/Memstream.h

)

add_library(PaperCommon STATIC

	${PaperCommon_SOURCES}

)

qt5_use_modules(PaperCommon Core Gui Svg)
