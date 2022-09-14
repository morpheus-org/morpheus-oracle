function(morpheusoracle_append_config_line LINE)
  global_append(MORPHEUSORACLE_TPL_EXPORTS "${LINE}")
endfunction()

macro(MORPHEUSORACLE_ADD_TPL_OPTION NAME DEFAULT_VALUE DOCSTRING)
  cmake_parse_arguments(TPL "" "DEFAULT_DOCSTRING" "" ${ARGN})
  if(NOT TPL_DEFAULT_DOCSTRING)
    set(TPL_DEFAULT_DOCSTRING "${DEFAULT_VALUE}")
  endif()

  set(_NAME_ORIG ${NAME})
  set(_NAME ${NAME})

  morpheusoracle_add_option(ENABLE_TPL_${_NAME} ${DEFAULT_VALUE} BOOL
                            "${DOCSTRING} Default: ${TPL_DEFAULT_DOCSTRING}")
  set(ROOT_DEFAULT $ENV{${_NAME_ORIG}_ROOT})
  morpheusoracle_add_option(
    ${_NAME_ORIG}_ROOT
    "${ROOT_DEFAULT}"
    PATH
    "Location of ${_NAME} install root. Default: None or the value of the environment variable ${_NAME}_ROOT if set"
  )
  if(DEFINED TPL_ENABLE_${_NAME})
    if(TPL_ENABLE_${_NAME} AND NOT MORPHEUSORACLE_ENABLE_TPL_${_NAME})
      message(
        "Overriding MORPHEUSORACLE_ENABLE_TPL_${_NAME_ORIG}=OFF with TPL_ENABLE_${_NAME}=ON"
      )
      set(MORPHEUSORACLE_ENABLE_TPL_${_NAME_ORIG} ON)
      set(MORPHEUSORACLE_ENABLE_TPL_${_NAME} ON)
    elseif(NOT TPL_ENABLE_${_NAME} AND MORPHEUSORACLE_ENABLE_TPL_${_NAME})
      message(
        "Overriding MORPHEUSORACLE_ENABLE_TPL_${_NAME_ORIG}=ON with TPL_ENABLE_${_NAME}=OFF"
      )
      set(MORPHEUSORACLE_ENABLE_TPL_${_NAME_ORIG} OFF)
      set(MORPHEUSORACLE_ENABLE_TPL_${_NAME} OFF)
    endif()
  endif()
  if(MORPHEUSORACLE_ENABLE_TPL_${_NAME})
    list(APPEND MORPHEUSORACLE_TPL_LIST ${_NAME})
  endif()
endmacro()

macro(morpheusoracle_create_imported_tpl NAME)
  cmake_parse_arguments(
    TPL "INTERFACE" "LIBRARY;IMPORTED_NAME"
    "LINK_LIBRARIES;INCLUDES;COMPILE_OPTIONS;LINK_OPTIONS" ${ARGN})

  if(NOT TPL_IMPORTED_NAME)
    set(TPL_IMPORTED_NAME MorpheusOracle::${NAME})
  endif()

  set(TPL_${NAME}_IMPORTED_NAME ${TPL_IMPORTED_NAME})

  if(TPL_INTERFACE)
    add_library(${NAME} INTERFACE)
    # Give this an importy-looking name
    add_library(${TPL_IMPORTED_NAME} ALIAS ${NAME})
    if(TPL_LIBRARY)
      message(
        SEND_ERROR
          "TPL Interface library ${NAME} should not have an IMPORTED_LOCATION")
    endif()
    # Things have to go in quoted in case we have multiple list entries
    if(TPL_LINK_LIBRARIES)
      target_link_libraries(${NAME} INTERFACE ${TPL_LINK_LIBRARIES})
      if(NOT DEFINED ${NAME}_FOUND_INFO)
        set(${NAME}_FOUND_INFO ${TPL_LINK_LIBRARIES})
      endif()
    endif()
    if(TPL_INCLUDES)
      target_include_directories(${NAME} INTERFACE ${TPL_INCLUDES})
      if(NOT DEFINED ${NAME}_FOUND_INFO)
        set(${NAME}_FOUND_INFO ${TPL_INCLUDES})
      endif()
    endif()
    if(TPL_COMPILE_OPTIONS)
      target_compile_options(${NAME} INTERFACE ${TPL_COMPILE_OPTIONS})
    endif()
    if(TPL_LINK_OPTIONS)
      target_link_libraries(${NAME} INTERFACE ${TPL_LINK_OPTIONS})
    endif()
  else()
    add_library(${TPL_IMPORTED_NAME} UNKNOWN IMPORTED)
    if(TPL_LIBRARY)
      set_target_properties(${TPL_IMPORTED_NAME} PROPERTIES IMPORTED_LOCATION
                                                            ${TPL_LIBRARY})
      if(NOT DEFINED ${NAME}_FOUND_INFO)
        set(${NAME}_FOUND_INFO ${TPL_LIBRARY})
      endif()
    endif()
    # Things have to go in quoted in case we have multiple list entries
    if(TPL_LINK_LIBRARIES)
      set_target_properties(
        ${TPL_IMPORTED_NAME} PROPERTIES INTERFACE_LINK_LIBRARIES
                                        "${TPL_LINK_LIBRARIES}")
      if(NOT DEFINED ${NAME}_FOUND_INFO)
        set(${NAME}_FOUND_INFO ${TPL_LINK_LIBRARIES})
      endif()
    endif()
    if(TPL_INCLUDES)
      set_target_properties(
        ${TPL_IMPORTED_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES
                                        "${TPL_INCLUDES}")
      if(NOT DEFINED ${NAME}_FOUND_INFO)
        set(${NAME}_FOUND_INFO ${TPL_INCLUDES})
      endif()
    endif()
    if(TPL_COMPILE_OPTIONS)
      set_target_properties(
        ${TPL_IMPORTED_NAME} PROPERTIES INTERFACE_COMPILE_OPTIONS
                                        "${TPL_COMPILE_OPTIONS}")
    endif()
    if(TPL_LINK_OPTIONS)
      set_target_properties(
        ${TPL_IMPORTED_NAME} PROPERTIES INTERFACE_LINK_LIBRARIES
                                        "${TPL_LINK_OPTIONS}")
    endif()
  endif()
endmacro()

macro(morpheusoracle_find_header VAR_NAME HEADER TPL_NAME)
  cmake_parse_arguments(TPL "ALLOW_SYSTEM_PATH_FALLBACK" "" "PATHS" ${ARGN})

  set(${VAR_NAME} "${HEADER}-NOTFOUND")
  set(HAVE_CUSTOM_PATHS FALSE)
  if(NOT ${VAR_NAME} AND ${TPL_NAME}_ROOT)
    # ONLY look in the root directory
    find_path(
      ${VAR_NAME} ${HEADER}
      PATHS ${${TPL_NAME}_ROOT}/include
      NO_DEFAULT_PATH)
    set(HAVE_CUSTOM_PATHS TRUE)
  endif()

  if(NOT ${VAR_NAME} AND MORPHEUSORACLE_${TPL_NAME}_ROOT)
    # ONLY look in the root directory
    find_path(
      ${VAR_NAME} ${HEADER}
      PATHS ${MORPHEUSORACLE_${TPL_NAME}_ROOT}/include
      NO_DEFAULT_PATH)
    set(HAVE_CUSTOM_PATHS TRUE)
  endif()

  if(NOT ${VAR_NAME} AND TPL_PATHS)
    # we got custom paths ONLY look in these paths and nowhere else
    find_path(
      ${VAR_NAME} ${HEADER}
      PATHS ${TPL_PATHS}
      NO_DEFAULT_PATH)
    set(HAVE_CUSTOM_PATHS TRUE)
  endif()

  if(NOT HAVE_CUSTOM_PATHS OR TPL_ALLOW_SYSTEM_PATH_FALLBACK)
    # Now go ahead and look in system paths
    if(NOT ${VAR_NAME})
      find_path(${VAR_NAME} ${HEADER})
    endif()
  endif()
endmacro()

macro(morpheusoracle_find_library VAR_NAME TPL_NAME)
  cmake_parse_arguments(TPL "ALLOW_SYSTEM_PATH_FALLBACK" ""
                        "PATHS;LIBRARY_NAMES" ${ARGN})

  set(${VAR_NAME} "${TPL_NAME}-NOTFOUND")
  set(HAVE_CUSTOM_PATHS FALSE)
  if(NOT ${VAR_NAME} AND ${TPL_NAME}_ROOT)
    find_library(
      ${VAR_NAME}
      NAMES ${TPL_LIBRARY_NAMES}
      PATHS ${${TPL_NAME}_ROOT}/lib ${${TPL_NAME}_ROOT}/lib64
      NO_DEFAULT_PATH)
    set(HAVE_CUSTOM_PATHS TRUE)
  endif()

  if(NOT ${VAR_NAME} AND MORPHEUSORACLE_${TPL_NAME}_ROOT)
    # we got root paths, only look in these paths and nowhere else
    find_library(
      ${VAR_NAME}
      NAMES ${TPL_LIBRARY_NAMES}
      PATHS ${MORPHEUSORACLE_${TPL_NAME}_ROOT}/lib
            ${MORPHEUSORACLE_${TPL_NAME}_ROOT}/lib64
      NO_DEFAULT_PATH)
    set(HAVE_CUSTOM_PATHS TRUE)
  endif()

  if(NOT ${VAR_NAME} AND TPL_PATHS)
    # we got custom paths, only look in these paths and nowhere else
    find_library(
      ${VAR_NAME}
      NAMES ${TPL_LIBRARY_NAMES}
      PATHS ${TPL_PATHS}
      NO_DEFAULT_PATH)
    set(HAVE_CUSTOM_PATHS TRUE)
  endif()

  if(NOT HAVE_CUSTOM_PATHS OR TPL_ALLOW_SYSTEM_PATH_FALLBACK)
    if(NOT ${VAR_NAME})
      # Now go ahead and look in system paths
      find_library(${VAR_NAME} NAMES ${TPL_LIBRARY_NAMES})
    endif()
  endif()

endmacro()

macro(morpheusoracle_find_imported NAME)
  cmake_parse_arguments(
    TPL "INTERFACE;ALLOW_SYSTEM_PATH_FALLBACK" "HEADER;IMPORTED_NAME"
    "LIBRARY;HEADERS;LIBRARIES;HEADER_PATHS;LIBRARY_PATHS" ${ARGN})
  # LIBRARY can be a list of possible library names matching the NAMES keyword
  # to CMake find_library

  if(NOT TPL_MODULE_NAME)
    set(TPL_MODULE_NAME TPL${NAME})
  endif()

  if(TPL_ALLOW_SYSTEM_PATH_FALLBACK)
    set(ALLOW_PATH_FALLBACK_OPT ALLOW_SYSTEM_PATH_FALLBACK)
  else()
    set(ALLOW_PATH_FALLBACK_OPT)
  endif()

  if(NOT TPL_IMPORTED_NAME)
    set(TPL_IMPORTED_NAME MorpheusOracle::${NAME})
  endif()

  set(${NAME}_INCLUDE_DIRS)
  if(TPL_HEADER)
    morpheusoracle_find_header(
      ${NAME}_INCLUDE_DIRS ${TPL_HEADER} ${NAME} ${ALLOW_PATH_FALLBACK_OPT}
      PATHS ${TPL_HEADER_PATHS})
  endif()

  foreach(HEADER ${TPL_HEADERS})
    morpheusoracle_find_header(
      HEADER_FIND_TEMP ${HEADER} ${NAME} ${ALLOW_PATH_FALLBACK_OPT} PATHS
      ${TPL_HEADER_PATHS})
    if(HEADER_FIND_TEMP)
      list(APPEND ${NAME}_INCLUDE_DIRS ${HEADER_FIND_TEMP})
    endif()
  endforeach()

  set(${NAME}_LIBRARY)
  if(TPL_LIBRARY)
    morpheusoracle_find_library(
      ${NAME}_LIBRARY
      ${NAME}
      ${ALLOW_PATH_FALLBACK_OPT}
      LIBRARY_NAMES
      ${TPL_LIBRARY}
      PATHS
      ${TPL_LIBRARY_PATHS})
  endif()

  set(${NAME}_FOUND_LIBRARIES)
  # We must find every library in this list
  foreach(LIB ${TPL_LIBRARIES})
    # we want the actual name, not the name -lblas, etc
    set(LIB_CLEAN ${LIB})
    string(FIND "${LIB}" "-l" PREFIX_IDX)
    if(PREFIX_IDX STREQUAL "0")
      string(SUBSTRING ${LIB} 2 -1 LIB_CLEAN)
    endif()

    morpheusoracle_find_library(
      ${LIB}_LOCATION
      ${NAME}
      ${ALLOW_PATH_FALLBACK_OPT}
      LIBRARY_NAMES
      ${LIB_CLEAN}
      PATHS
      ${TPL_LIBRARY_PATHS})
    if(${LIB}_LOCATION)
      list(APPEND ${NAME}_FOUND_LIBRARIES ${${LIB}_LOCATION})
    else()
      set(${NAME}_FOUND_LIBRARIES ${${LIB}_LOCATION})
      break()
    endif()
  endforeach()

  include(FindPackageHandleStandardArgs)
  set(TPL_VARS_NEEDED)
  if(TPL_LIBRARY)
    list(APPEND TPL_VARS_NEEDED ${NAME}_LIBRARY)
  endif()
  if(TPL_HEADER)
    list(APPEND TPL_VARS_NEEDED ${NAME}_INCLUDE_DIRS)
  endif()
  if(TPL_HEADERS)
    list(APPEND TPL_VARS_NEEDED ${NAME}_INCLUDE_DIRS)
  endif()
  if(TPL_LIBRARIES)
    list(APPEND TPL_VARS_NEEDED ${NAME}_FOUND_LIBRARIES)
  endif()
  find_package_handle_standard_args(${TPL_MODULE_NAME}
                                    REQUIRED_VARS ${TPL_VARS_NEEDED})

  if(${TPL_MODULE_NAME}_FOUND)
    set(IMPORT_TYPE)
    if(TPL_INTERFACE)
      set(IMPORT_TYPE "INTERFACE")
    endif()
    morpheusoracle_create_imported_tpl(
      ${NAME}
      ${IMPORT_TYPE}
      IMPORTED_NAME
      ${TPL_IMPORTED_NAME}
      INCLUDES
      "${${NAME}_INCLUDE_DIRS}"
      LIBRARY
      "${${NAME}_LIBRARY}"
      LINK_LIBRARIES
      "${${NAME}_FOUND_LIBRARIES}")
  endif()
  # This is a macro, clear variables we don't to escape
  set(TPL_MODULE_NAME)
endmacro()

macro(morpheusoracle_export_imported_tpl NAME)
  cmake_parse_arguments(TPL "" "IMPORTED_NAME" "" ${ARGN})
  if(NOT TPL_IMPORTED_NAME)
    set(TPL_IMPORTED_NAME MorpheusOracle::${NAME})
  endif()

  get_target_property(LIB_TYPE ${TPL_IMPORTED_NAME} TYPE)
  if(${LIB_TYPE} STREQUAL "INTERFACE_LIBRARY")
    # This is not an imported target This an interface library that we created
    install(
      TARGETS ${NAME}
      EXPORT MorpheusOracleTargets
      RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
  endif()
endmacro()

macro(morpheusoracle_import_tpl NAME)
  set(${NAME}_LIBRARIES
      ""
      CACHE
        STRING
        "Optional override for the libraries that comprise TPL ${NAME}. Default: None. Default common library names will be searched"
  )
  set(${NAME}_LIBRARY_DIRS
      ""
      CACHE
        STRING
        "Optional override for the library directories that comprise TPL ${NAME}. Default: None. Default common library locations will be searched"
  )
  set(${NAME}_INCLUDE_DIRS
      ""
      CACHE
        STRING
        "Optional override for the header directories that comprise TPL ${NAME}. Default: None. Default common header locations will be searched"
  )

  cmake_parse_arguments(TPL "NO_EXPORT" "" "" ${ARGN})

  # Even though this policy gets set in the top-level CMakeLists.txt, I have
  # still been getting errors about ROOT variables being ignored I'm not sure if
  # this is a scope issue - but make sure the policy is set before we do any
  # find_package calls
  cmake_policy(SET CMP0074 NEW)

  if(MORPHEUSORACLE_ENABLE_TPL_${NAME})
    # Tack on a TPL here to make sure we avoid using anyone else's find
    find_package(TPL${NAME} REQUIRED MODULE)
    if(NOT TPL_${NAME}_IMPORTED_NAME)
      message(
        FATAL_ERROR
          "Find module did not produce valid IMPORTED_NAME for ${NAME}")
    endif()

    if(NOT TARGET ${TPL_${NAME}_IMPORTED_NAME})
      message(
        FATAL_ERROR
          "Find module succeeded for ${NAME}, but did not produce valid target ${TPL_${NAME}_IMPORTED_NAME}"
      )
    endif()
    if(NOT TPL_NO_EXPORT)
      morpheusoracle_export_imported_tpl(${NAME} IMPORTED_NAME
                                         ${TPL_${NAME}_IMPORTED_NAME})
    endif()
  endif()
endmacro(morpheusoracle_import_tpl)

function(morpheusoracle_link_tpl TARGET)
  cmake_parse_arguments(TPL "PUBLIC;PRIVATE;INTERFACE" "IMPORTED_NAME" ""
                        ${ARGN})
  # the name of the TPL
  set(TPL ${TPL_UNPARSED_ARGUMENTS})

  if(NOT TPL_IMPORTED_NAME)
    set(TPL_IMPORTED_NAME MorpheusOracle::${TPL})
  endif()
  if(MORPHEUSORACLE_ENABLE_TPL_${TPL})
    if(TPL_PUBLIC)
      target_link_libraries(${TARGET} PUBLIC ${TPL_IMPORTED_NAME})
    elseif(TPL_PRIVATE)
      target_link_libraries(${TARGET} PRIVATE ${TPL_IMPORTED_NAME})
    elseif(TPL_INTERFACE)
      target_link_libraries(${TARGET} INTERFACE ${TPL_IMPORTED_NAME})
    else()
      target_link_libraries(${TARGET} ${TPL_IMPORTED_NAME})
    endif()
  endif()
endfunction()

# Convert list to newlines (which CMake doesn't always like in cache variables)
string(REPLACE ";" "\n" MORPHEUSORACLE_TPL_EXPORT_TEMP
               "${MORPHEUSORACLE_TPL_EXPORTS}")
# Convert to a regular variable
unset(MORPHEUSORACLE_TPL_EXPORTS CACHE)
set(MORPHEUSORACLE_TPL_EXPORTS ${MORPHEUSORACLE_TPL_EXPORT_TEMP})
