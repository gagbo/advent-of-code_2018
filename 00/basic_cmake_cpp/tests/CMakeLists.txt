
include(FetchContent)

fetchcontent_declare(catch
                     GIT_REPOSITORY
                     https://github.com/catchorg/Catch2
                     GIT_TAG
                     v2.5.0)

fetchcontent_getproperties(catch)
if(NOT catch_POPULATED)
  fetchcontent_populate(catch)
  message(STATUS "Catch source dir: ${catch_SOURCE_DIR}")
  message(STATUS "Catch binary dir: ${catch_BINARY_DIR}")
  add_subdirectory(${catch_SOURCE_DIR} ${catch_BINARY_DIR})
endif()

add_executable(tests test_lib.cpp test_main.cpp)
target_link_libraries(tests PUBLIC baselib)
target_link_libraries(tests PRIVATE Catch2::Catch2)
target_compile_features(tests PUBLIC cxx_std_17)

include(CTest)
set(CMAKE_MODULE_PATH "${catch_SOURCE_DIR}/contrib" ${CMAKE_MODULE_PATH})
include(Catch)

catch_discover_tests(tests TEST_SPEC *)
