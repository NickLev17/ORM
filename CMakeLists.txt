cmake_minimum_required(VERSION 3.14)

project(main VERSION 0.0.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(database SHARED
    src/MySQLDatabase.cpp
    src/SQLiteDatabase.cpp
)
target_include_directories(database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(orm_database src/main.cpp)
target_include_directories(orm_database PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(orm_database database mysqlcppconn sqlite3)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_SOURCES
    test/test_main.cpp
    test/test_sqlite.cpp
)

add_executable(runTests ${TEST_SOURCES})
target_link_libraries(runTests PRIVATE database GTest::gtest_main pthread sqlite3 mysqlcppconn)
target_include_directories(runTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(runTests)

install(TARGETS database
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(TARGETS orm_database
    RUNTIME DESTINATION bin
)

if(NOT DEFINED PATCH_VERSION)
  set(PATCH_VERSION "0")
endif()

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "orm-database")
set(CPACK_PACKAGE_FILE_NAME "orm_database-0.0.${PATCH_VERSION}-Linux")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_CONTACT "example@example.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "example@example.com")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.7), libmysqlcppconn8v5, libsqlite3-0")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "ORM Database shared library and tools")

include(CPack)
