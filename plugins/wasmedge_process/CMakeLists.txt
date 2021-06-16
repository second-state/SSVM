# SPDX-License-Identifier: Apache-2.0

add_library(wasmedgePluginWasmEdgeProcess
  MODULE
  processfunc.cpp
  processmodule.cpp
)

target_include_directories(wasmedgePluginWasmEdgeProcess
  PUBLIC
  ${CMAKE_SOURCE_DIR}/include
  $<TARGET_PROPERTY:wasmedgeCommon,INCLUDE_DIRECTORIES>
)

add_library(wasmedgeHostModuleWasmEdgeProcess
  processfunc.cpp
  processmodule.cpp
)

target_include_directories(wasmedgeHostModuleWasmEdgeProcess
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(wasmedgeHostModuleWasmEdgeProcess
  PUBLIC
  wasmedgeAST
)
