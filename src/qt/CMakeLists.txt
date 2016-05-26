project(wfxs-qt)

find_package(Qt4 REQUIRED)

set(wfxsQt_SOURCES
	XSQtApplication.cpp
	XSQtCanvas.cpp
	XSQtEditWindow.cpp
	XSQtPropertiesDialog.cpp
	main.cpp
)
set(wfxsQt_HEADERS
	XSQtApplication.h
	XSQtCanvas.h
	XSQtEditWindow.h
	XSQtPropertiesDialog.h
)

QT4_WRAP_CPP(wfxsQt_HEADERS_MOC ${wfxsQt_HEADERS})

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

add_executable(wfxs-qt ${wfxsQt_SOURCES} ${wfxsQt_HEADERS_MOC})
target_link_libraries(wfxs-qt ${QT_LIBRARIES} wfxs)
