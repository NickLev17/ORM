cmake_minimum_required(VERSION 3.5)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})


project(main VERSION ${PROJECT_VERSION})
configure_file(version.h.in version.h)

set(SOURCES
    src/main.cpp
   src/MySQLDatabase.cpp
   src/SQLiteDatabase.cpp
)

set(HEADERS
    include/MySQLDatabase.h
    include/SQLiteDatabase.h
    include/Database.h
)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include) 




add_executable(main ${SOURCES})
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)



target_link_libraries(main
    mysqlcppconn  # MySQL Connector/C++ библиотека
    sqlite3       # SQLite3 библиотека
)
