morpheusoracle_include_directories(${CMAKE_CURRENT_BINARY_DIR})
morpheusoracle_include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(run-first-tuner)
add_subdirectory(decision-tree-tuner)
add_subdirectory(random-forest-tuner)
