set(PaperCommon_SOURCES

	Functionality.cpp
	Functionality.h
	QRCode.cpp
	QRCode.h

)

add_library(PaperCommon STATIC

	${PaperCommon_SOURCES}

)

qt5_use_modules(PaperCommon Core Gui Svg)
