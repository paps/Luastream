set(CMAKE_CXX_CUSTOM_FLAGS "-Wall -Wextra -std=c++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_CUSTOM_FLAGS}")

find_package(Boost COMPONENTS system REQUIRED)
find_package(Lua51 REQUIRED)
find_package(Luabind REQUIRED)

file(GLOB server_src "*.[ch]pp")
file(GLOB common_src "../common/*.[ch]pp")

include_directories(
    ".."
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LUA_INCLUDE_DIR}
    ${LUABIND_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)

list(APPEND src
    ${server_src}
    ${common_src}
)

add_executable(server
    ${src}
)

target_link_libraries(server
    ${LUA_LIBRARIES}
    ${LUABIND_LIBRARY}
    ${Boost_LIBRARIES}
)
