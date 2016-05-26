project(wfxs-gtk)

set(wfxsGtk_SOURCES
    XSGtkApplication.cpp
    XSGtkCanvas.cpp
    XSGtkEditWindow.cpp
    main.cpp
)

set(wfxsGtk_HEADERS
    XSGtkApplication.h
    XSGtkCanvas.h
    XSGtkEditWindow.h
)

include_directories(wfxs-gtk ${GTKMM_INCLUDE_DIRS})
link_directories(wfxs-gtk ${GTKMM_LIBRARY_DIRS})
add_executable(wfxs-gtk ${wfxsGtk_SOURCES} ${wfxsGtk_HEADERS})
target_link_libraries(wfxs-gtk ${GTKMM_LIBRARIES} wfxs)
