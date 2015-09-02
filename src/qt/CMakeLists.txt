project(wfxs-qt)

find_package(Qt4 REQUIRED)

set(wfxsQt_SOURCES
	main.cpp
	XSQtEditWindow.cpp
	XSQtCanvas.cpp
	XSQtPropertiesDialog.cpp
)
set(wfxsQt_HEADERS
	XSQtEditWindow.h
	XSQtCanvas.h
	XSQtPropertiesDialog.h
)

QT4_WRAP_CPP(wfxsQt_HEADERS_MOC ${wfxsQt_HEADERS})

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

add_executable(wfxs-qt ${wfxsQt_SOURCES} ${wfxsQt_HEADERS_MOC})
target_link_libraries(wfxs-qt ${QT_LIBRARIES} wfxs)
