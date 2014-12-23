set(PaperTests_SOURCES

	PaperTests.cpp

	Tests/CompressionTest.cpp
	Tests/CompressionTest.h

)

include_directories(${CMAKE_SOURCE_DIR}/vrfy/src)

add_executable(PaperTests ${PaperTests_SOURCES})
target_link_libraries(PaperTests ${Paper_LIBS} ${Vrfy_LIBS})

qt5_use_modules(PaperTests Core Gui Svg)
