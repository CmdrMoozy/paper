set(PaperCLI_SOURCES

	PaperCLI.cpp
	PaperCLI.h

)

add_executable(PaperCLI ${PaperCLI_SOURCES})
target_link_libraries(PaperCLI ${Paper_LIBS})

qt5_use_modules(PaperCLI Core Gui Svg)
