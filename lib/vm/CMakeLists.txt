# SPDX-License-Identifier: Apache-2.0

add_library(wasmedgeVM
  vm.cpp
)

target_compile_options(wasmedgeVM
  PRIVATE
  ${WASMEDGE_CFLAGS}
)

target_link_libraries(wasmedgeVM
  PUBLIC
  wasmedgeCommon
  wasmedgeAST
  wasmedgeLoader
  wasmedgeValidator
  wasmedgeInterpreter
  wasmedgeHostModuleWasi
  wasmedgeHostModuleWasmEdgeProcess
)
