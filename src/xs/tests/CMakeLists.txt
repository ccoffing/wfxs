include_directories(
    ..
)

add_executable(XSCommandTest XSCommandTest.cpp catch.hpp)
target_link_libraries(XSCommandTest wfxs)
add_test(NAME XSCommandTest COMMAND XSCommandTest)

add_executable(XSColorTest XSColorTest.cpp catch.hpp)
target_link_libraries(XSColorTest wfxs)
add_test(NAME XSColorTest COMMAND XSColorTest)

add_executable(XSFlossTest XSFlossTest.cpp catch.hpp)
target_link_libraries(XSFlossTest wfxs)
add_test(NAME XSFlossTest COMMAND XSFlossTest)

add_executable(XSModelTest XSModelTest.cpp catch.hpp)
target_link_libraries(XSModelTest wfxs)
add_test(NAME XSModelTest COMMAND XSModelTest)

add_executable(XSSkeinTest XSSkeinTest.cpp catch.hpp)
target_link_libraries(XSSkeinTest wfxs)
add_test(NAME XSSkeinTest COMMAND XSSkeinTest)
