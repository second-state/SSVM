# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.9.4)
cmake_policy(SET CMP0091 NEW)
project(WasmEdge)

find_program(GIT_CMD git)
execute_process(COMMAND
  ${GIT_CMD} describe --match "*[0-9].[0-9]*" --tag
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE CPACK_PACKAGE_VERSION
  RESULT_VARIABLE GIT_VERSION_NOT_FOUND
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(GIT_VERSION_NOT_FOUND AND NOT GIT_VERSION_NOT_FOUND EQUAL 0)
  set(CPACK_PACKAGE_VERSION "0.0.0-unreleased")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

include(FetchContent)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Filesystem REQUIRED Final Experimental)
find_package(Threads REQUIRED)

find_package(Boost QUIET)
if(${Boost_FOUND})
else()
  FetchContent_Declare(
    Boost
    URL https://boostorg.jfrog.io/artifactory/main/release/1.76.0/source/boost_1_76_0.tar.bz2
    URL_HASH SHA256=f0397ba6e982c4450f27bf32a2a83292aba035b827a5623a14636ea583318c41
  )
  set(BOOST_ENABLE_CMAKE ON)
  set(BOOST_RUNTIME_LINK static)
  set(BUILD_TESTING OFF)
  FetchContent_MakeAvailable(Boost)
  add_library(Boost_boost INTERFACE)
  add_library(Boost::boost ALIAS Boost_boost)
  target_include_directories(Boost_boost INTERFACE ${boost_SOURCE_DIR})
endif()

if(CMAKE_BUILD_TYPE STREQUAL Release OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
  if (CMAKE_GENERATOR STREQUAL Ninja)
    if(CMAKE_COMPILER_IS_GNUCXX)
      list(TRANSFORM CMAKE_C_COMPILE_OPTIONS_IPO REPLACE "^-flto$" "-flto=auto")
      list(TRANSFORM CMAKE_CXX_COMPILE_OPTIONS_IPO REPLACE "^-flto$" "-flto=auto")
    endif()
    set(CMAKE_JOB_POOLS "link=2")
    set(CMAKE_JOB_POOL_LINK link)
  endif()
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

list(APPEND WASMEDGE_CFLAGS
  -Wall
  -Wextra
  -Werror
  -Wno-error=pedantic
  -Wno-psabi
)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  list(APPEND WASMEDGE_CFLAGS
    -Wno-c++20-designator
    -Wno-c99-extensions
    -Wno-covered-switch-default
    -Wno-documentation-unknown-command
    -Wno-keyword-macro
    -Wno-newline-eof
    -Wno-return-std-move-in-c++11
    -Wno-shadow-field-in-constructor
    -Wno-signed-enum-bitfield
    -Wno-switch-enum
    -Wno-undefined-func-template
  )
endif()

if(WIN32)
  add_definitions(-D_ENABLE_EXTENDED_ALIGNED_STORAGE -DNOMINMAX -D_ITERATOR_DEBUG_LEVEL=0)
  list(APPEND WASMEDGE_CFLAGS
    "/EHa"
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-exit-time-destructors
    -Wno-global-constructors
    -Wno-used-but-marked-unused
    -Wno-nonportable-system-include-path
    -Wno-float-equal
  )
endif()
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_ENABLE_EXPORTS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_SKIP_RPATH ON)

# List of WasmEdge options
option(BUILD_TESTS "Generate build targets for the wasmedge unit tests." OFF)
option(BUILD_COVERAGE "Generate coverage report. Require BUILD_TESTS." OFF)
option(BUILD_AOT_RUNTIME "Enable WasmEdge LLVM-based ahead of time compilation runtime." ON)
option(BUILD_SHARED_LIB "Generate the WasmEdge shared library." ON)
option(BUILD_TOOLS "Generate wasmedge and wasmedgec tools." ON)
option(BUILD_TOOL_WASMEDGE_STATIC "Generate wasmedge-static tool" OFF)
set(BUILD_PACKAGE "DEB;RPM" CACHE STRING "Package generate types")
set(CPACK_PROJECT_CONFIG_FILE ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cpack_config.cmake)

if(BUILD_COVERAGE)
  include(CodeCoverage)
  append_coverage_compiler_flags()
endif()

include(GNUInstallDirs)

# Macro for copying directory.
macro(configure_files srcDir destDir)
  message(STATUS "Generating testing input directory ${destDir}")
  make_directory(${destDir})

  file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*)
  foreach(templateFile ${templateFiles})
    set(srcTemplatePath ${srcDir}/${templateFile})
    if(NOT IS_DIRECTORY ${srcTemplatePath})
      configure_file(
        ${srcTemplatePath}
        ${destDir}/${templateFile}
        COPYONLY)
    else()
      configure_files(
        ${srcTemplatePath}
        ${destDir}/${templateFile}
        COPYONLY)
    endif()
  endforeach(templateFile)
endmacro(configure_files)
# End of Macro

string(TOUPPER "${CMAKE_INSTALL_DEFAULT_COMPONENT_NAME}" CMAKE_INSTALL_DEFAULT_COMPONENT_NAME_UPCASE)
set(CPACK_PACKAGE_VENDOR Second State LLC)
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")
set(CPACK_STRIP_FILES ON)
set(CPACK_PACKAGE_CONTACT "Shen-Ta Hsieh <beststeve@secondstate.io>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "High performance WebAssembly Virtual Machine")
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_ARCHIVE_${CMAKE_INSTALL_DEFAULT_COMPONENT_NAME_UPCASE}_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}")
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_RPM_MAIN_COMPONENT ${CMAKE_INSTALL_DEFAULT_COMPONENT_NAME})
set(CPACK_RPM_PACKAGE_LICENSE "Apache 2.0")
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_${CMAKE_INSTALL_DEFAULT_COMPONENT_NAME_UPCASE}_PACKAGE_NAME wasmedge)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEBIAN_COMPRESSION_TYPE xz)
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/WasmEdge/WasmEdge/")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION ON)
if(BUILD_TOOL_WASMEDGE_STATIC)
  set(CPACK_DEBIAN_STATIC_PACKAGE_RECOMMENDS "binfmt-support | systemd")
  set(CPACK_DEBIAN_STATIC_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/binfmt/postinst;${CMAKE_CURRENT_SOURCE_DIR}/binfmt/prerm")
endif()
set(CPACK_GENERATOR "${BUILD_PACKAGE}")
set(CPACK_PACKAGE_DESCRIPTION "WasmEdge is a high performance, extensible, and hardware optimized WebAssembly Virtual Machine for cloud, AI, and blockchain applications.")

if(BUILD_TESTS)
  include(CTest)
  add_subdirectory(test)
endif()

add_subdirectory(include)
add_subdirectory(lib)
add_subdirectory(thirdparty)
if(BUILD_TOOLS)
  if(BUILD_TOOL_WASMEDGE_STATIC)
    add_subdirectory(binfmt)
  endif()
  add_subdirectory(tools)
endif()

include(CPack)
include(CPackComponent)
if(BUILD_TOOL_WASMEDGE_STATIC)
  cpack_add_component_group(static)
  cpack_add_component(static GROUP static)
  cpack_add_component(static_debian GROUP static)
endif()
