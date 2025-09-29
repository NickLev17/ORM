cmake_minimum_required(VERSION 3.14)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(main VERSION ${PROJECT_VERSION})
configure_file(version.h.in version.h)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Основная библиотека
add_library(database SHARED
    src/MySQLDatabase.cpp
    src/SQLiteDatabase.cpp
)
target_include_directories(database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
install(TARGETS database
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Основной исполняемый файл
add_executable(orm_database src/main.cpp)
target_include_directories(orm_database PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(orm_database
    database
    mysqlcppconn
    sqlite3
)

include(FetchContent)

# Подключение GoogleTest с веткой main
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Тестовые исходники без включения googletest в библиотеку
set(TEST_SOURCES
    test/test_main.cpp
    test/test_sqlite.cpp
)

add_executable(runTests ${TEST_SOURCES})
target_link_libraries(runTests PRIVATE database GTest::gtest_main pthread sqlite3)
target_include_directories(runTests PRIVATE include)

include(GoogleTest)
gtest_discover_tests(runTests)

install(TARGETS orm_database RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
