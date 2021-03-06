# SPDX-License-Identifier: Apache-2.0
configure_files(
  ${CMAKE_CURRENT_SOURCE_DIR}/examples
  ${CMAKE_CURRENT_BINARY_DIR}/examples
  COPYONLY
)

if (BUILD_AOT_RUNTIME)
  add_executable(wasmedgec
    wasmedgec.cpp
  )

  target_compile_options(wasmedgec
    PRIVATE
    ${WASMEDGE_CFLAGS}
  )

  install(TARGETS wasmedgec EXPORT wasmedge DESTINATION bin)

  target_link_libraries(wasmedgec
    PRIVATE
    wasmedgeLoader
    wasmedgeCommon
    wasmedgeValidator
    wasmedgeAOT
    std::filesystem
  )
endif()

add_executable(wasmedge
  wasmedger.cpp
)

target_compile_options(wasmedge
  PRIVATE
  ${WASMEDGE_CFLAGS}
)

install(TARGETS wasmedge EXPORT wasmedge DESTINATION bin)

target_link_libraries(wasmedge
  PRIVATE
  wasmedgeVM
)

if(BUILD_TOOL_WASMEDGE_STATIC)
  add_executable(wasmedge-static
    wasmedger-static.cpp
  )

  target_compile_options(wasmedge-static
    PRIVATE
    ${WASMEDGE_CFLAGS}
  )

  install(TARGETS wasmedge-static COMPONENT static EXPORT wasmedge-static DESTINATION bin)

  target_link_libraries(wasmedge-static
    PRIVATE
    wasmedgeVM
    -static
  )
endif()
