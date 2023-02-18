include(CMakeParseArguments)
include(CTest)

cmake_policy(SET CMP0054 NEW)

message(STATUS "The project name is: ${PROJECT_NAME}")

macro(GLOBAL_SET VARNAME)
  set(${VARNAME}
      ${ARGN}
      CACHE INTERNAL "")
endmacro()

function(VERIFY_EMPTY CONTEXT)
  if(${ARGN})
    message(FATAL_ERROR "Unhandled arguments in ${CONTEXT}:\n${ARGN}")
  endif()
endfunction()

function(GLOBAL_APPEND VARNAME)
  # We make this a function since we are setting variables and want to use scope
  # to avoid overwriting local variables
  set(TEMP ${${VARNAME}})
  list(APPEND TEMP ${ARGN})
  global_set(${VARNAME} ${TEMP})
endfunction()

macro(APPEND_GLOB VAR)
  file(GLOB LOCAL_TMP_VAR ${ARGN})
  list(APPEND ${VAR} ${LOCAL_TMP_VAR})
endmacro()

macro(MORPHEUSORACLE_PACKAGE)
  set(PACKAGE_NAME MorpheusOracle)
  set(PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
  string(TOUPPER ${PACKAGE_NAME} PACKAGE_NAME_UC)
  set(${PACKAGE_NAME}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endmacro(MORPHEUSORACLE_PACKAGE)

macro(MORPHEUSORACLE_PACKAGE_POSTPROCESS)
  include(CMakePackageConfigHelpers)
  configure_package_config_file(
    cmake/MorpheusOracleConfig.cmake.in
    "${MorpheusOracle_BINARY_DIR}/MorpheusOracleConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MorpheusOracle)
  write_basic_package_version_file(
    "${MorpheusOracle_BINARY_DIR}/MorpheusOracleConfigVersion.cmake"
    VERSION
      "${MorpheusOracle_VERSION_MAJOR}.${MorpheusOracle_VERSION_MINOR}.${MorpheusOracle_VERSION_PATCH}"
    COMPATIBILITY SameMajorVersion)

  install(FILES "${MorpheusOracle_BINARY_DIR}/MorpheusOracleConfig.cmake"
                "${MorpheusOracle_BINARY_DIR}/MorpheusOracleConfigVersion.cmake"
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MorpheusOracle)

  install(
    EXPORT MorpheusOracleTargets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/MorpheusOracle
    NAMESPACE Morpheus::)
endmacro(MORPHEUSORACLE_PACKAGE_POSTPROCESS)

function(MORPHEUSORACLE_CONFIGURE_FILE PACKAGE_NAME_CONFIG_FILE)
  # Configure the file
  configure_file(${PACKAGE_SOURCE_DIR}/cmake/${PACKAGE_NAME_CONFIG_FILE}.in
                 ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME_CONFIG_FILE})
endfunction(MORPHEUSORACLE_CONFIGURE_FILE)

function(MORPHEUSORACLE_INCLUDE_DIRECTORIES)
  cmake_parse_arguments(INC "REQUIRED_DURING_INSTALLATION_TESTING" "" ""
                        ${ARGN})
  include_directories(${INC_UNPARSED_ARGUMENTS})
endfunction(MORPHEUSORACLE_INCLUDE_DIRECTORIES)

function(MORPHEUSORACLE_LIB_TYPE LIB RET)
  get_target_property(PROP ${LIB} TYPE)
  if(${PROP} STREQUAL "INTERFACE_LIBRARY")
    set(${RET}
        "INTERFACE"
        PARENT_SCOPE)
  else()
    set(${RET}
        "PUBLIC"
        PARENT_SCOPE)
  endif()
endfunction()

function(MORPHEUSORACLE_LIB_INCLUDE_DIRECTORIES TARGET)
  # append to a list for later
  morpheusoracle_lib_type(${TARGET} INCTYPE)
  foreach(DIR ${ARGN})
    target_include_directories(${TARGET} ${INCTYPE} $<BUILD_INTERFACE:${DIR}>)
  endforeach()
endfunction(MORPHEUSORACLE_LIB_INCLUDE_DIRECTORIES)

macro(MORPHEUSORACLE_ADD_TEST_DIRECTORIES)
  if(MORPHEUSORACLE_ENABLE_TESTS)
    foreach(TEST_DIR ${ARGN})
      add_subdirectory(${TEST_DIR})
    endforeach()
  endif()
endmacro(MORPHEUSORACLE_ADD_TEST_DIRECTORIES)

macro(MORPHEUSORACLE_ADD_EXAMPLE_DIRECTORIES)
  if(MORPHEUSORACLE_ENABLE_EXAMPLES)
    foreach(EXAMPLE_DIR ${ARGN})
      add_subdirectory(${EXAMPLE_DIR})
    endforeach()
  endif()
endmacro(MORPHEUSORACLE_ADD_EXAMPLE_DIRECTORIES)

macro(MORPHEUSORACLE_ADD_BENCHMARK_DIRECTORIES)
  if(MORPHEUSORACLE_ENABLE_BENCHMARKS)
    foreach(BENCHMARK_DIR ${ARGN})
      add_subdirectory(${BENCHMARK_DIR})
    endforeach()
  endif()
endmacro(MORPHEUSORACLE_ADD_BENCHMARK_DIRECTORIES)

function(morpheusoracle_add_option SUFFIX DEFAULT TYPE DOCSTRING)
  cmake_parse_arguments(
    OPT "" "" "VALID_ENTRIES" # if this is a list variable, the valid values in
                              # the list
    ${ARGN})

  set(CAMEL_NAME MorpheusOracle_${SUFFIX})
  string(TOUPPER ${CAMEL_NAME} UC_NAME)

  # Make sure this appears in the cache with the appropriate DOCSTRING
  set(${CAMEL_NAME}
      ${DEFAULT}
      CACHE ${TYPE} ${DOCSTRING})

  # I don't love doing it this way because it's N^2 in number options, but cest
  # la vie
  foreach(opt ${MORPHEUSORACLE_GIVEN_VARIABLES})
    string(TOUPPER ${opt} OPT_UC)
    if("${OPT_UC}" STREQUAL "${UC_NAME}")
      if(NOT "${opt}" STREQUAL "${CAMEL_NAME}")
        message(
          FATAL_ERROR
            "Matching option found for ${CAMEL_NAME} with the wrong case ${opt}. Please delete your CMakeCache.txt and change option to -D${CAMEL_NAME}=${${opt}}. This is now enforced to avoid hard-to-debug CMake cache inconsistencies."
        )
      endif()
    endif()
  endforeach()

  # okay, great, we passed the validation test - use the default
  if(DEFINED ${CAMEL_NAME})
    if(OPT_VALID_ENTRIES)
      string(TOUPPER "${OPT_VALID_ENTRIES}" OPT_VALID_ENTRIES_UC)
      foreach(entry ${${CAMEL_NAME}})
        string(TOUPPER ${entry} ENTRY_UC)
        if(NOT ${ENTRY_UC} IN_LIST OPT_VALID_ENTRIES_UC)
          message(
            FATAL_ERROR
              "Given entry ${entry} in list for option ${SUFFIX}. "
              "Valid case-insensitive values are any of ${OPT_VALID_ENTRIES}")
        endif()
      endforeach()
      string(TOUPPER "${${CAMEL_NAME}}" GIVEN_ENTRIES_UC)
      set(${UC_NAME}
          ${GIVEN_ENTRIES_UC}
          PARENT_SCOPE)
    else()
      set(${UC_NAME}
          ${${CAMEL_NAME}}
          PARENT_SCOPE)
    endif()
  else()
    set(${UC_NAME}
        ${DEFAULT}
        PARENT_SCOPE)
  endif()

endfunction()

function(MORPHEUSORACLE_ADD_LIBRARY LIBRARY_NAME)
  morpheusoracle_internal_add_library(${LIBRARY_NAME} ${ARGN})
endfunction()

# function(MORPHEUSORACLE_ADD_EXECUTABLE EXE_NAME) cmake_parse_arguments(PARSE
# "" "" "SOURCES;COMPONENTS;TESTONLYLIBS" ${ARGN})
# verify_empty(MORPHEUSORACLE_ADD_EXECUTABLE ${PARSE_UNPARSED_ARGUMENTS})

# morpheusoracle_is_enabled(COMPONENTS ${PARSE_COMPONENTS} OUTPUT_VARIABLE
# IS_ENABLED)

# if(IS_ENABLED) add_executable(${EXE_NAME} ${PARSE_SOURCES}) # AJP, BMK
# altered: if(MORPHEUSORACLE_ENABLE_TESTS_AND_PERFSUITE)
# target_link_libraries(${EXE_NAME} PRIVATE common ${PARSE_TESTONLYLIBS})
# endif()

# if(PARSE_TESTONLYLIBS) target_link_libraries(${EXE_NAME} PRIVATE
# Morpheus::morpheus-oracle ${PARSE_TESTONLYLIBS}) else()
# target_link_libraries(${EXE_NAME} PRIVATE Morpheus::morpheus-oracle) endif()
# else() message( STATUS "Skipping executable ${EXE_NAME} because not all
# necessary components enabled" ) endif() endfunction()

# function(MORPHEUSORACLE_ADD_EXECUTABLE_AND_TEST ROOT_NAME)

# cmake_parse_arguments(PARSE "" "" "SOURCES;CATEGORIES;COMPONENTS;TESTONLYLIBS"
# ${ARGN}) verify_empty(MORPHEUSORACLE_ADD_EXECUTABLE_AND_RUN_VERIFY
# ${PARSE_UNPARSED_ARGUMENTS})

# morpheusoracle_is_enabled(COMPONENTS ${PARSE_COMPONENTS} OUTPUT_VARIABLE
# IS_ENABLED)

# if(IS_ENABLED) set(EXE_NAME ${PACKAGE_NAME}_${ROOT_NAME})
# morpheusoracle_add_executable(${EXE_NAME} SOURCES ${PARSE_SOURCES})
# if(PARSE_TESTONLYLIBS) target_link_libraries(${EXE_NAME} PRIVATE
# ${PARSE_TESTONLYLIBS}) endif() morpheusoracle_add_test(NAME ${ROOT_NAME} EXE
# ${EXE_NAME}) else() message( STATUS "Skipping executable/test ${ROOT_NAME}
# because not all necessary components enabled" ) endif()

# endfunction()

# function(MORPHEUSORACLE_ADD_TEST)

# cmake_parse_arguments(PARSE "" "" "COMPONENTS" ${ARGN})

# morpheusoracle_is_enabled(COMPONENTS ${PARSE_COMPONENTS} OUTPUT_VARIABLE
# IS_ENABLED)

# if(IS_ENABLED) cmake_parse_arguments( TEST "WILL_FAIL"
# "FAIL_REGULAR_EXPRESSION;PASS_REGULAR_EXPRESSION;EXE;NAME" "CATEGORIES"
# ${PARSE_UNPARSED_ARGUMENTS}) if(TEST_EXE) set(EXE ${TEST_EXE}) else() set(EXE
# ${TEST_NAME}) endif() if(WIN32) add_test( NAME ${TEST_NAME} WORKING_DIRECTORY
# ${LIBRARY_OUTPUT_PATH} COMMAND ${EXE}${CMAKE_EXECUTABLE_SUFFIX}) else()
# add_test(NAME ${TEST_NAME} COMMAND ${EXE}) endif() if(TEST_WILL_FAIL)
# set_tests_properties(${TEST_NAME} PROPERTIES WILL_FAIL ${TEST_WILL_FAIL})
# endif() if(TEST_FAIL_REGULAR_EXPRESSION) set_tests_properties( ${TEST_NAME}
# PROPERTIES FAIL_REGULAR_EXPRESSION ${TEST_FAIL_REGULAR_EXPRESSION}) endif()
# if(TEST_PASS_REGULAR_EXPRESSION) set_tests_properties( ${TEST_NAME} PROPERTIES
# PASS_REGULAR_EXPRESSION ${TEST_PASS_REGULAR_EXPRESSION}) endif()
# verify_empty(MORPHEUSORACLE_ADD_TEST ${TEST_UNPARSED_ARGUMENTS}) else()
# message( STATUS "Skipping test ${TEST_NAME} because not all necessary
# components enabled" ) endif() endfunction()

function(MORPHEUSORACLE_ADD_EXECUTABLE_AND_TEST ROOT_NAME)
  cmake_parse_arguments(PARSE "" "" "SOURCES;CATEGORIES;ARGS" ${ARGN})
  verify_empty(MORPHEUSORACLE_ADD_EXECUTABLE_AND_TEST
               ${PARSE_UNPARSED_ARGUMENTS})

  morpheusoracle_add_test_executable(${ROOT_NAME} SOURCES ${PARSE_SOURCES})
  if(PARSE_ARGS)
    set(TEST_NUMBER 0)
    foreach(ARG_STR ${PARSE_ARGS})
      # This is passed as a single string blob to match TriBITS behavior We need
      # this to be turned into a list
      string(REPLACE " " ";" ARG_STR_LIST ${ARG_STR})
      list(APPEND TEST_NAME "${ROOT_NAME}${TEST_NUMBER}")
      math(EXPR TEST_NUMBER "${TEST_NUMBER} + 1")
      morpheusoracle_add_test(
        NAME
        ${TEST_NAME}
        EXE
        ${ROOT_NAME}
        FAIL_REGULAR_EXPRESSION
        "  FAILED  "
        ARGS
        ${ARG_STR_LIST})
    endforeach()
  else()
    morpheusoracle_add_test(NAME ${ROOT_NAME} EXE ${ROOT_NAME}
                            FAIL_REGULAR_EXPRESSION "  FAILED  ")
  endif()
endfunction()

macro(MORPHEUSORACLE_ADD_TEST_EXECUTABLE ROOT_NAME)
  cmake_parse_arguments(PARSE "" "" "SOURCES" ${ARGN})
  morpheusoracle_add_executable(
    ${ROOT_NAME} SOURCES ${PARSE_SOURCES} ${PARSE_UNPARSED_ARGUMENTS}
    TESTONLYLIBS morpheusoracle_gtest)
  set(EXE_NAME ${PACKAGE_NAME}_${ROOT_NAME})
endmacro()

function(MORPHEUSORACLE_ADD_TEST)
  cmake_parse_arguments(
    TEST "WILL_FAIL;"
    "FAIL_REGULAR_EXPRESSION;PASS_REGULAR_EXPRESSION;EXE;NAME;TOOL"
    "CATEGORIES;ARGS" ${ARGN})
  if(TEST_EXE)
    set(EXE_ROOT ${TEST_EXE})
  else()
    set(EXE_ROOT ${TEST_NAME})
  endif()
  # Prepend package name to the test name These should be the full target name
  set(TEST_NAME ${PACKAGE_NAME}_${TEST_NAME})
  set(EXE ${PACKAGE_NAME}_${EXE_ROOT})
  if(WIN32)
    add_test(
      NAME ${TEST_NAME}
      WORKING_DIRECTORY ${LIBRARY_OUTPUT_PATH}
      COMMAND ${EXE}${CMAKE_EXECUTABLE_SUFFIX} ${TEST_ARGS})
  else()
    add_test(NAME ${TEST_NAME} COMMAND ${EXE} ${TEST_ARGS})
  endif()
  if(TEST_WILL_FAIL)
    set_tests_properties(${TEST_NAME} PROPERTIES WILL_FAIL ${TEST_WILL_FAIL})
  endif()
  if(TEST_FAIL_REGULAR_EXPRESSION)
    set_tests_properties(
      ${TEST_NAME} PROPERTIES FAIL_REGULAR_EXPRESSION
                              ${TEST_FAIL_REGULAR_EXPRESSION})
  endif()
  if(TEST_PASS_REGULAR_EXPRESSION)
    set_tests_properties(
      ${TEST_NAME} PROPERTIES PASS_REGULAR_EXPRESSION
                              ${TEST_PASS_REGULAR_EXPRESSION})
  endif()
  verify_empty(MORPHEUSORACLE_ADD_TEST ${TEST_UNPARSED_ARGUMENTS})
endfunction()

function(MORPHEUSORACLE_ADD_EXECUTABLE ROOT_NAME)
  cmake_parse_arguments(PARSE "TESTONLY" "" "SOURCES;TESTONLYLIBS" ${ARGN})

  set(EXE_NAME ${PACKAGE_NAME}_${ROOT_NAME})
  add_executable(${EXE_NAME} ${PARSE_SOURCES})
  if(PARSE_TESTONLYLIBS)
    target_link_libraries(${EXE_NAME} PRIVATE ${PARSE_TESTONLYLIBS})
  endif()
  verify_empty(MORPHEUSORACLE_ADD_EXECUTABLE ${PARSE_UNPARSED_ARGUMENTS})
  # All executables must link to all the morpheus targets This is just private
  # linkage because exe is final
  target_link_libraries(${EXE_NAME} PRIVATE Morpheus::morpheus-oracle)
endfunction()

function(MORPHEUSORACLE_INTERNAL_ADD_LIBRARY LIBRARY_NAME)
  cmake_parse_arguments(PARSE "STATIC;SHARED" "" "HEADERS;SOURCES" ${ARGN})

  if(PARSE_HEADERS)
    list(REMOVE_DUPLICATES PARSE_HEADERS)
  endif()
  if(PARSE_SOURCES)
    list(REMOVE_DUPLICATES PARSE_SOURCES)
  endif()

  add_library(${LIBRARY_NAME} ${PARSE_HEADERS} ${PARSE_SOURCES})
  add_library(Morpheus::${LIBRARY_NAME} ALIAS ${LIBRARY_NAME})

  install(
    TARGETS ${LIBRARY_NAME}
    EXPORT ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT ${PACKAGE_NAME})

  install(
    TARGETS ${LIBRARY_NAME}
    EXPORT MorpheusOracleTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endfunction(MORPHEUSORACLE_INTERNAL_ADD_LIBRARY LIBRARY_NAME)

function(MORPHEUSORACLE_IS_ENABLED)
  cmake_parse_arguments(PARSE "" "OUTPUT_VARIABLE" "COMPONENTS" ${ARGN})

  if(MORPHEUSORACLE_ENABLED_COMPONENTS STREQUAL "ALL")
    set(${PARSE_OUTPUT_VARIABLE}
        TRUE
        PARENT_SCOPE)
  elseif(PARSE_COMPONENTS)
    set(ENABLED TRUE)
    foreach(comp ${PARSE_COMPONENTS})
      string(TOUPPER ${comp} COMP_UC)
      # make sure this is in the list of enabled components
      if(NOT "${COMP_UC}" IN_LIST MORPHEUSORACLE_ENABLED_COMPONENTS)
        # if not in the list, one or more components is missing
        set(ENABLED FALSE)
      endif()
    endforeach()
    set(${PARSE_OUTPUT_VARIABLE}
        ${ENABLED}
        PARENT_SCOPE)
  else()
    # we did not enable all components and no components were given as part of
    # this - we consider this enabled
    set(${PARSE_OUTPUT_VARIABLE}
        TRUE
        PARENT_SCOPE)
  endif()
endfunction()
