project(wfxs-wx)

file(GLOB wfcsWxSources *.cpp *.h)

add_executable(wfxs-wx ${wfxsWx_SOURCES})
target_link_libraries(wfxs-wx wfxs clc)
list(APPEND wfcsLibDirs ${WX_LIBRARY_DIRS})
