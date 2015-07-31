project(wfxs-gtk)

file(GLOB wfxsGtk_SOURCES *.cpp *.h)

add_executable(wfxs-gtk ${wfxsGtk_SOURCES})
target_link_libraries(wfxs-gtk ${GTKMM_LIBRARIES} wfxs clc)
list(APPEND wfcsLibDirs ${GTKMM_LIBRARY_DIRS})
