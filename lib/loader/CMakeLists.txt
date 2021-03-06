# SPDX-License-Identifier: Apache-2.0

add_library(wasmedgeLoaderFileMgr
  filemgr.cpp
  ldmgr.cpp
  shared_library.cpp
)

target_link_libraries(wasmedgeLoaderFileMgr
  PUBLIC
  wasmedgeCommon
  wasmedgeSystem
  std::filesystem
)

target_compile_options(wasmedgeLoaderFileMgr
  PRIVATE
  ${WASMEDGE_CFLAGS}
)

if(NOT WIN32)
  target_link_libraries(wasmedgeLoaderFileMgr
    PUBLIC
    dl
  )
endif()

add_library(wasmedgeLoader
  loader.cpp
)

target_compile_options(wasmedgeLoader
  PRIVATE
  ${WASMEDGE_CFLAGS}
)

target_link_libraries(wasmedgeLoader
  PUBLIC
  wasmedgeCommon
  wasmedgeAST
  wasmedgeLoaderFileMgr
  Boost::boost
  std::filesystem
)
