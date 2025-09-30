cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(main VERSION ${PROJECT_VERSION})
configure_file(version.h.in version.h)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(database SHARED
    src/MySQLDatabase.cpp
    src/SQLiteDatabase.cpp
)
target_include_directories(database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
install(TARGETS database
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
add_executable(orm_database src/main.cpp)
target_include_directories(orm_database PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(orm_database
    database
    mysqlcppconn
    sqlite3
)

enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(TEST_SOURCES
    test/test_main.cpp
    test/test_sqlite.cpp
    src/SQLiteDatabase.cpp
)

add_executable(runTests ${TEST_SOURCES})
target_link_libraries(runTests PRIVATE database ${GTEST_LIBRARIES} pthread sqlite3)
target_include_directories(runTests PRIVATE include)

add_test(NAME runTests COMMAND runTests)



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
