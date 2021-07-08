# SPDX-License-Identifier: Apache-2.0

wasmedge_add_executable(wasmedgeLoaderFileMgrTests
  filemgrTest.cpp
)

add_test(wasmedgeLoaderFileMgrTests wasmedgeLoaderFileMgrTests)

wasmedge_add_executable(wasmedgeLoaderWagonTests
  wagonTest.cpp
)

add_test(wasmedgeLoaderWagonTests wasmedgeLoaderWagonTests)

wasmedge_add_executable(wasmedgeLoaderEthereumTests
  ethereumTest.cpp
)

add_test(wasmedgeLoaderEthereumTests wasmedgeLoaderEthereumTests)

file(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/filemgrTestData
  DESTINATION
  ${CMAKE_CURRENT_BINARY_DIR}
)
file(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/wagonTestData
  DESTINATION
  ${CMAKE_CURRENT_BINARY_DIR}
)
file(COPY
  ${CMAKE_CURRENT_SOURCE_DIR}/ethereumTestData
  DESTINATION
  ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(wasmedgeLoaderFileMgrTests
  PRIVATE
  ${GTEST_BOTH_LIBRARIES}
  wasmedgeLoaderFileMgr
)

target_link_libraries(wasmedgeLoaderWagonTests
  PRIVATE
  ${GTEST_BOTH_LIBRARIES}
  wasmedgeLoaderFileMgr
  wasmedgeAST
)

target_link_libraries(wasmedgeLoaderEthereumTests
  PRIVATE
  ${GTEST_BOTH_LIBRARIES}
  wasmedgeLoaderFileMgr
  wasmedgeAST
)
