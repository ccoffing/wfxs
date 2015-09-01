project(wfxs-haiku)

set(wfxsHaiku_SOURCES
    XSAboutWindow.cpp
    XSApp_beos.cpp
    XSEditFlossWindow.cpp
    XSPropertiesWindow.cpp
    XSStartupWizard.cpp
    XSToolWindow.cpp
    XSView_beos.cpp
    XSWindowBeos.cpp
    XSWindowMenuUpdater.cpp
)
set(wfxsHaiku_HEADERS
    XSAboutWindow.h
    XSEditFlossWindow.h
    XSPropertiesWindow.h
    XSStartupWizard.h
    XSToolWindow.h
    XSView_beos.h
    XSWindowBeos.h
    XSWindowMenuUpdater.h
)

add_executable(wfxs-haiku ${wfxsHaiku_SOURCES} ${wfxsHaiku_HEADERS})
target_link_libraries(wfxs-haiku wfxs)
