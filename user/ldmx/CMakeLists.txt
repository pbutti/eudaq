get_filename_component(EUDAQ_USERNAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(EUDAQ_MODULE module_${EUDAQ_USERNAME})

option(USER_EXAMPLE_BUILD "build user/ldmx" ON)

if(NOT USER_EXAMPLE_BUILD)
    message(STATUS "user/ldmx is NOT to be built (USER_EXAMPLE_BUILD=OFF)")
  return()
endif()

message(STATUS "user/ldmx is to be built (USER_EXAMPLE_BUILD=ON)")

add_subdirectory(module)
