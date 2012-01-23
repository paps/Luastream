set(CMAKE_CXX_CUSTOM_FLAGS "-Wall -Wextra -std=c++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_CUSTOM_FLAGS}")

find_package(Boost COMPONENTS system REQUIRED)
find_package(SFML 1.6 REQUIRED window graphics)

file(GLOB client_src "*.[ch]pp")
file(GLOB common_src "../common/*.[ch]pp")

include_directories(
    ".."
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SFML_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)

list(APPEND src
    ${client_src}
    ${common_src}
)

add_executable(client
    ${src}
)

target_link_libraries(client
    ${SFML_LIBRARIES}
    ${Boost_LIBRARIES}
)
