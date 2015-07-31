project(wfxs-wx)

file(GLOB wfxsWx_SOURCES *.cpp *.h)

add_executable(wfxs-wx ${wfxsWx_SOURCES})
target_link_libraries(wfxs-wx wfxs clc)
list(APPEND wfxsLibDirs ${WX_LIBRARY_DIRS})
