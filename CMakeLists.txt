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
    mysqlcppconn  
    sqlite3       
)
find_package(Boost 1.70 CONFIG REQUIRED COMPONENTS unit_test_framework)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})

    
    target_link_libraries(main Boost::unit_test_framework)
else()
    message(FATAL_ERROR "Boost not found")
endif()
install(TARGETS orm RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)
