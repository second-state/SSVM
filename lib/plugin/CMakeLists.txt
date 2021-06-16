# SPDX-License-Identifier: Apache-2.0

add_library(wasmedgePlugin
  plugin.cpp
)

target_link_libraries(wasmedgePlugin
  PUBLIC
  wasmedgeCommon
  wasmedgeLoaderFileMgr
)

target_include_directories(wasmedgePlugin
  PUBLIC
  ${Boost_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/thirdparty
)
