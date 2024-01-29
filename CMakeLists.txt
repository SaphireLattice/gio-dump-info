cmake_minimum_required(VERSION 3.5)
project(giotest)

find_package(PkgConfig REQUIRED)
pkg_check_modules(glib REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(glib-unix REQUIRED IMPORTED_TARGET gio-unix-2.0)

add_executable(giotest test.c)

target_link_libraries(giotest PkgConfig::glib PkgConfig::glib-unix)