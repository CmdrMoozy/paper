set(PaperCommon_SOURCES

	Functionality.cpp
	Functionality.h

	Compression/LZMA.cpp
	Compression/LZMA.h

	QR/Coding.cpp
	QR/Coding.h
	QR/QRCode.cpp
	QR/QRCode.h

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
