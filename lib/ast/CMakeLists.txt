# SPDX-License-Identifier: Apache-2.0

add_library(wasmedgeAST
  module.cpp
  section.cpp
  description.cpp
  segment.cpp
  type.cpp
  expression.cpp
  instruction.cpp
)

target_compile_options(wasmedgeAST
  PRIVATE
  ${WASMEDGE_CFLAGS}
)

target_link_libraries(wasmedgeAST
  PUBLIC
  wasmedgeLoaderFileMgr
  wasmedgeCommon
  wasmedgeLoaderFileMgr
)
