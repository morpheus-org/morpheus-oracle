morpheusoracle_configure_file(MorpheusOracle_config.hpp)

morpheusoracle_include_directories(${CMAKE_CURRENT_BINARY_DIR}
                                   ${CMAKE_CURRENT_SOURCE_DIR})

set(MORPHEUS_ORACLE_HEADER_INSTALL_DIR ${MORPHEUS_HEADER_INSTALL_DIR}/oracle)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
  DESTINATION ${MORPHEUS_ORACLE_HEADER_INSTALL_DIR}
  FILES_MATCHING
  PATTERN "*.hpp"
  PATTERN "*.h")

set(MORPHEUS_ORACLE_SOURCES)
append_glob(MORPHEUS_ORACLE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(MORPHEUS_ORACLE_HEADERS)
append_glob(MORPHEUS_ORACLE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
append_glob(MORPHEUS_ORACLE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/impl/*.hpp)
append_glob(MORPHEUS_ORACLE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/fwd/*.hpp)

foreach(Tuner RunFirst;DecisionTree;RandomForest)
  append_glob(MORPHEUS_ORACLE_SOURCES
              ${CMAKE_CURRENT_SOURCE_DIR}/impl/${Tuner}/*.cpp)
  append_glob(MORPHEUS_ORACLE_HEADERS
              ${CMAKE_CURRENT_SOURCE_DIR}/impl/${Tuner}/*.hpp)
endforeach()

append_glob(MORPHEUS_ORACLE_HEADERS
            ${CMAKE_CURRENT_BINARY_DIR}/MorpheusOracle_config.hpp)

foreach(HEADER ${MORPHEUS_ORACLE_HEADERS})
  get_filename_component(dirname ${HEADER} DIRECTORY)
  morpheusoracle_include_directories(${dirname})
endforeach()

morpheusoracle_add_library(morpheus-oracle SOURCES ${MORPHEUS_ORACLE_SOURCES}
                           HEADERS ${MORPHEUS_ORACLE_HEADERS})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/MorpheusOracle_config.hpp
        DESTINATION ${MORPHEUS_ORACLE_HEADER_INSTALL_DIR})

morpheusoracle_lib_include_directories(
  morpheus-oracle ${MORPHEUSORACLE_TOP_BUILD_DIR} ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR})

foreach(HEADER ${MORPHEUS_ORACLE_HEADERS})
  get_filename_component(dirname ${HEADER} DIRECTORY)
  target_include_directories(morpheus-oracle
                             PUBLIC $<BUILD_INTERFACE:${dirname}>)
endforeach()
target_include_directories(
  morpheus-oracle
  PUBLIC $<INSTALL_INTERFACE:${MORPHEUS_ORACLE_HEADER_INSTALL_DIR}>)
