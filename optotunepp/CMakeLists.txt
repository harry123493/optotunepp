project(optotunepp)

add_library(
    ${PROJECT_NAME}
    src/sp.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)