morpheusoracle_configure_file(MorpheusOracle_config.h)

morpheusoracle_include_directories(${CMAKE_CURRENT_BINARY_DIR})
morpheusoracle_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(MORPHEUS_ORACLE_HEADER_INSTALL_DIR ${MORPHEUS_HEADER_INSTALL_DIR}/oracle)
set(MORPHEUSORACLE_HEADER_INSTALL_DIR ${MORPHEUS_ORACLE_HEADER_INSTALL_DIR})

set(MORPHEUS_ORACLE_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND MORPHEUS_ORACLE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

# I don't want the complexity of sometimes interface/sometimes not Just
# minimally force an empty dummy file
set(SOURCES dummy.cpp)

# -----------------------------------------------------------------------------

list(APPEND MORPHEUS_ORACLE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND MORPHEUS_ORACLE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/impl)
list(APPEND MORPHEUS_ORACLE_INCLUDE_DIRS
     ${CMAKE_CURRENT_SOURCE_DIR}/impl/RunFirst)

foreach(DIR ${MORPHEUS_ORACLE_INCLUDE_DIRS})
  morpheusoracle_include_directories(${DIR})
  append_glob(HEADERS ${DIR}/*.hpp)
endforeach()

list(APPEND HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${PACKAGE_NAME}_config.h)

morpheusoracle_add_library(morpheus-oracle HEADERS ${HEADERS} SOURCES
                           ${SOURCES})

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
  DESTINATION ${MORPHEUS_ORACLE_HEADER_INSTALL_DIR}
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.h")

add_library(Morpheus::morpheus-oracle ALIAS morpheus-oracle)
target_link_libraries(morpheus-oracle PUBLIC Morpheus::morpheus)
foreach(DIR ${MORPHEUS_ORACLE_INCLUDE_DIRS})
  target_include_directories(morpheus-oracle PUBLIC $<BUILD_INTERFACE:${DIR}>)
endforeach()
target_include_directories(
  morpheus-oracle
  PUBLIC $<INSTALL_INTERFACE:${MORPHEUS_ORACLE_HEADER_INSTALL_DIR}>)
